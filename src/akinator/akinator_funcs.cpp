#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <tree_funcs.h>
#include <tree_debug.h>
#include <akinator_funcs.h>

static int VOICE = 0;

static int del_new_line_sym(Buffer *buffer);

static int print_def(Node *node);

static int get_str(char *str);

static int write_pedigree(Node **pedigree, Node *node, int depth);

static int update_base(Node *node);

static int akinator_print_indent(int indent);

static FILE *BASE = 0;

int akinator_handle_base(const char* path_to_file, Buffer *buff)
{   
    SOFT_ASS(path_to_file == NULL);
    SOFT_ASS(buff == NULL);
    
    BASE = fopen(path_to_file, "rw+");

    if(NULL == BASE)                  //todo akinator verificator
    {
        printf("cant open buffer file");
        return -1;
    }

    buff->size = akinator_size_for_buffer(BASE, path_to_file);

    buff->buffer = (char*) calloc(buff->size, sizeof(char));        //todo verify
    assert(buff->buffer != NULL);

    int test_fread = 0;
    test_fread = fread(buff->buffer, sizeof(char), buff->size, BASE);      //todo verify
    assert(test_fread != 0);  

    del_new_line_sym(buff);

    buff->buffer[buff->size] = '\0';

    buff->curr_index = 0;

    return 0;
}

static int del_new_line_sym(Buffer* buff)
{
    SOFT_ASS(buff == NULL);

    for (int i = 0; i < buff->size; i++)
    {
        if (buff->buffer[i] == '\n')
        {
            buff->buffer[i] = '\0';
        }
    }

    return 0;
}

int akinator_size_for_buffer(FILE *BASE, const char *path_to_file)
{
    SOFT_ASS(BASE == NULL);
    SOFT_ASS(path_to_file == NULL);


    struct stat buf ={};
    stat(path_to_file, &buf);

    return buf.st_size + 1;
}


int akinator_do_tree(Node *node, Buffer *buff)
{   
    SOFT_ASS(node == NULL);
    SOFT_ASS(buff == NULL);

    char new_str[LEN_OF_DATA] = {};
    char *cur_pos = buff->buffer + buff->curr_index;
     
    if (buff->curr_index >= buff->size)
    {
        return 0;
    }

    if (strchr(cur_pos, '{'))
    {   
        node->data = strchr(cur_pos, '#') + 1;
    }

    buff->curr_index += strlen(cur_pos) + 1;
    if (!strchr(cur_pos, '}'))
    {   
        link_nodes(node, buff);
    }
    return 0;
}

int link_nodes(Node *node, Buffer *buff)
{
    SOFT_ASS(node == NULL);
    SOFT_ASS(buff == NULL);

    char new_str[LEN_OF_DATA] = {};

    if (buff->curr_index >= buff->size)
    {
        return 0;
    }
    
    //todo: use this variable instead 
    char *cur_pos = buff->buffer + buff->curr_index;
    
    if (strchr(cur_pos, '{'))
        {   
            Node *l_node = node_ctor_connect(node, LEFT);

            if (strchr(cur_pos, '}'))
            {   
                strchr(cur_pos, '}')[0] = '\0';
                l_node->data = strchr(cur_pos, '#') + 1;
                
                buff->curr_index += strlen(buff->buffer + buff->curr_index) + 1;
                buff->curr_index += strlen(buff->buffer + buff->curr_index) + 1;
            }
            else
            {   
                akinator_do_tree(l_node, buff);
            }
            
            cur_pos = buff->buffer + buff->curr_index;
            Node *r_node = node_ctor_connect(node, RIGHT);
            if (strchr(cur_pos, '}'))
            {   
                strchr(cur_pos, '}')[0] = '\0';

                r_node->data = strchr(cur_pos, '#') + 1;

                buff->curr_index += strlen(buff->buffer + buff->curr_index) + 1;
                buff->curr_index += strlen(buff->buffer + buff->curr_index) + 1;
                
                while ((buff->curr_index <= buff->size - 1) && (strchr(buff->buffer + buff->curr_index, '{') == NULL))  
                {   
                    buff->curr_index += strlen(cur_pos) + 1;
                }

            }
            else
            {
                akinator_do_tree(r_node, buff);
            }

        }
    
    return 0;
}

int akinator_dtor(Buffer *buffer)
{   
    if (BASE != NULL)
    {
        fclose(BASE);
        BASE = NULL;
    }

    if (buffer->buffer != NULL)
    {
        free(buffer->buffer);
        buffer->buffer = NULL;
    }
    
    return 0;
}

int akinator_guess(Node *node)
{
    SOFT_ASS(node == NULL);
    
    char choice = ' ';
    Node *tmp_node = node;

    say_and_write("If you agree input \'y\', else input \'n\'");

    while(tmp_node->l_son != NULL)
    {   
        char str [512] = {};
        sprintf(str, "\n%s?", tmp_node->data);
        say_and_write(str);

        scanf(" %c", &choice);

        switch(choice)
        {
            case 'y':
            {
                tmp_node = tmp_node->l_son;
                break;
            }
            case 'n':
            {
                tmp_node = tmp_node->r_son;
                break;
            }
            default:
            {
                say_and_write("Why you so stupid?");
                reset_stdin();
            }
        }
    }

    char str [512] = {};
    sprintf(str,"Is it %s?", tmp_node->data);
    say_and_write(str);

    scanf(" %c", &choice);
    
    switch(choice)
        {
            case 'y':
            {
                say_and_write("Its so obvious");
                break;
            }
            case 'n':
            {
                akinator_add_node(tmp_node);
                break;
            }
            default:
            {
                say_and_write("Why you so stupid?");
                reset_stdin();
            }
        }

    return 0;
}

int akinator_add_node(Node *node)
{   
    SOFT_ASS(node == NULL);
    reset_stdin();

    char *new_obj = (char*) calloc(LEN_OF_DATA, sizeof(char));
    char *difference = (char*) calloc(LEN_OF_DATA, sizeof(char));

    SOFT_ASS(new_obj == NULL);
    SOFT_ASS(difference == NULL);
    
    say_and_write("Ok, what it was?\n");
    fgets(new_obj, LEN_OF_DATA, stdin);
    strchr(new_obj, '\n')[0] = '\0';

    char str[512] = {};
    sprintf(str, "What difference between %s and %s?\n", new_obj, node->data);
    say_and_write(str);

    fgets(difference, LEN_OF_DATA, stdin);
    strchr(difference, '\n')[0] = '\0';

    
    Node *l_node = node_ctor_connect(node, LEFT);           //todo func with add data
    Node *r_node = node_ctor_connect(node, RIGHT);
    
    l_node->data = new_obj;
    r_node->data = node->data;
    node->data = difference;
   
    return 0;
}

int reset_stdin()
{   
    char tmp_char = ' ';
    while ((tmp_char = getchar()) != '\n')
    {}

    return 0;
}

int akinator_update_base(Node *node)
{   
    SOFT_ASS(node == NULL);

    if(node->parent == NULL)
    {
        fseek(BASE, 0, SEEK_SET);   
    }

    update_base(node);

    return 0;
}

int update_base(Node *node)
{   
    
    if (!node)
    {
        return 0;
    }
    
    SOFT_ASS(node == NULL)
    SOFT_ASS(node->data == NULL);
    fprintf(BASE, "{ #%s", node->data);
    
    
    if(node->l_son)
    {   
        

        fprintf(BASE, "\n");
        update_base(node->l_son);
    }
    else    
    {   
        fprintf(BASE, "}");
        return 0;
    }

    if(node->r_son)
    {   
        
        fprintf(BASE, "\n");
        update_base(node->r_son);
    }
    else    
    {
        fprintf(BASE, "}");
        return 0;
    }
    
    fprintf(BASE,"\n}");
    


    return 0;
}

int akinator_define(Node *node)
{   
    SOFT_ASS(node == NULL);

    say_and_write("What do you want to define?");

    char str[LEN_OF_DATA] = {};
    fgets(str, LEN_OF_DATA, stdin);
    strchr(str, '\n')[0] = '\0';

    Node *proper_node = find_node(node, str);

    if (proper_node)
    {
        print_def(proper_node);
    }
    else    
    {
        say_and_write("We dont have this object");
    }

    return 0;
}

Node *find_node(Node *node, const char *str)
{   
    SOFT_ASS_NO_RET(node == NULL);
    SOFT_ASS_NO_RET(str == NULL);

    if (!node)
        return 0;

    Node *tmp_node = 0;

    if (strcmp(node->data, str) == 0)
    {   
        return node;
    }
    
    if (node->l_son)
    {   
        tmp_node = find_node(node->l_son, str);
    }

    if ((node->r_son) && (tmp_node == NULL))
    {
        tmp_node = find_node(node->r_son, str);
    }
    
    return tmp_node;
}

int print_def(Node *node)
{
    SOFT_ASS(node == NULL);

    if (node->parent)
    {
        print_def(node->parent);
    }

    if ((node->parent) && (node->parent->l_son == node))
    {
        say_and_write(node->parent->data);          //todo make it func and flag to speak
    }

    else if ((node->parent) && (node->parent->r_son == node))
    {   
        char str[LEN_OF_DATA] = {};
        sprintf(str, "not %s", node->parent->data);
        say_and_write(str);
    }

    return 0;
}

int akinator_compare(Node *node)
{   
    SOFT_ASS(node == NULL);

    char obj_1[LEN_OF_DATA] = {};
    char obj_2[LEN_OF_DATA] = {};
    Node *node1 = {};
    Node *pedigree1[128] = {};

    Node *node2 = {};
    Node *pedigree2[128] = {};

    say_and_write("Input first object for comparison:");
    get_str(obj_1);

    say_and_write("Input second object:");
    get_str(obj_2);

    node1 = find_node(node, obj_1);
    node2 = find_node(node, obj_2);

    if (!(node1 && node2))
    {
        say_and_write("You input wrong obj");
        return 0;
    }

    int depth1 = write_pedigree(pedigree1, node1, 0);
    pedigree1[depth1] = node1;

    int depth2 = write_pedigree(pedigree2, node2, 0);
    pedigree2[depth2] = node2;

    compare_pedigrees(pedigree1, pedigree2);

    return 0;
}

int write_pedigree(Node *pedigree[], Node *node, int depth)
{   
    SOFT_ASS(pedigree == NULL);

    if (!node)
    {   
        return 0;
    }
    
    if(node->parent) 
    {   
        depth = write_pedigree(pedigree, node->parent, depth);
        pedigree[depth] = node->parent;
        depth++;
    }

    return depth;
}

int compare_pedigrees(Node *pedigree1[], Node *pedigree2[])
{
    SOFT_ASS(pedigree1 == NULL);
    SOFT_ASS(pedigree2 == NULL);

    int index = 0;
    char TMP_STR[512] = {};

    while((pedigree1[index+1] != NULL) && (pedigree2[index+1] != NULL) && (pedigree1[index] == pedigree2[index]))
    {   
        if (pedigree1[index+1] == pedigree2[index + 1])
        {    
            sprintf(TMP_STR, "They both are%s%s", TEST_ON_NOT(index+1, pedigree1), pedigree1[index]->data);   
            say_and_write(TMP_STR);
        }
        else
        {   
            
            if (pedigree1[index+1]->pos == LEFT)
            {         
                sprintf(TMP_STR, "But first are%s%s", TEST_ON_NOT(index+1, pedigree1), pedigree1[index]->data);
                say_and_write(TMP_STR);
            }
            else
            {   
                sprintf(TMP_STR, "But second are%s%s", TEST_ON_NOT(index+1, pedigree2), pedigree2[index]->data);
                say_and_write(TMP_STR);
            }
        }
        index++;
    }
    
    int index1 = index;
    int index2 = index;

    if (pedigree1[index1 + 1] != NULL)
    {
        while(pedigree1[index1 + 1] != NULL)
        {
            sprintf(TMP_STR, "first also are%s%s", TEST_ON_NOT(index1+1, pedigree1), pedigree1[index1]->data);
            say_and_write(TMP_STR);
            index1++;
        }
    }
    
    if (pedigree2[index2 + 1] != NULL)
    {
        while(pedigree2[index2+1] != NULL)
        {   
            sprintf(TMP_STR, "second also are%s%s", TEST_ON_NOT(index2+1, pedigree2), pedigree2[index2]->data);
            say_and_write(TMP_STR);
            index2++;
        }
    }
    
    return 0;
}


int get_str(char *str)
{
    fgets(str, LEN_OF_DATA, stdin);
    strchr(str, '\n')[0] = '\0';

    return 0;
}

int say_and_write(const char *str)  
{                                                           
    printf("%s \n", str);                                       
    char command[512] = {};
    if(VOICE == 1)
    {
        sprintf(command, "festival -b \'(SayText \"%s\") \'", str); 
        system(command);                                            
    }

    return 0;
}   

int akinator_voice()
{
    puts("Do you need voice? (1 -- yes, 0 -- no)");

    scanf("%d", &VOICE);

    reset_stdin();

    return 0;
}

