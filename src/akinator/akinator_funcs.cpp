#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <tree_funcs.h>
#include <tree_debug.h>
#include <akinator_funcs.h>

static int del_new_line_sym(Buffer *buffer);

static int print_def(Node *node);

static int get_str(char *str);

static int write_pedigree(Node **pedigree, Node *node, int depth);

static FILE *BASE = 0;

int akinator_handle_base(const char* path_to_file, Buffer *buff)
{
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
    struct stat buf ={};
    stat(path_to_file, &buf);

    return buf.st_size + 1;
}


int akinator_do_tree(Node *node, Buffer *buff)
{   
    char new_str[LEN_OF_DATA] = {};
     
    if (buff->curr_index >= buff->size - 1)
    {
        return 0;
    }

    if (strchr(buff->buffer + buff->curr_index, '{'))
        {   
            node->data = strchr(buff->buffer + buff->curr_index, '#') + 1;
        }
    
    if (strchr(buff->buffer + buff->curr_index, '}'))
    {
        buff->curr_index += strlen(buff->buffer + buff->curr_index) + 1;        //todo remove from if and else
        return 0;
    }

    else
    {   
        buff->curr_index += strlen(buff->buffer + buff->curr_index) + 1;  
        link_nodes(node, buff);
    }
    return 0;
}

int link_nodes(Node *node, Buffer *buff)
{
    char new_str[LEN_OF_DATA] = {};

    if (buff->curr_index >= buff->size - 1)
    {
        return 0;
    }
    
    if (strchr(buff->buffer + buff->curr_index, '{'))
        {   
            Node *l_node = node_ctor_connect(node, LEFT);

            if (strchr(buff->buffer + buff->curr_index, '}'))
            {   
                strchr(buff->buffer + buff->curr_index, '}')[0] = '\0';
                l_node->data = strchr(buff->buffer + buff->curr_index, '#') + 1;
                
                buff->curr_index += strlen(buff->buffer + buff->curr_index) + 1;
                buff->curr_index += strlen(buff->buffer + buff->curr_index) + 1;
            }

            else
            {   
                akinator_do_tree(l_node, buff);
            }

            Node *r_node = node_ctor_connect(node, RIGHT);
            if (strchr(buff->buffer + buff->curr_index, '}'))
            {   
                strchr(buff->buffer + buff->curr_index, '}')[0] = '\0';

                r_node->data = strchr(buff->buffer + buff->curr_index, '#') + 1;

                buff->curr_index += strlen(buff->buffer + buff->curr_index) + 1;
                buff->curr_index += strlen(buff->buffer + buff->curr_index) + 1;
                
                while ((buff->curr_index <= buff->size -1) && (strchr(buff->buffer + buff->curr_index, '{') == NULL))
                {   
                    buff->curr_index += strlen(buff->buffer + buff->curr_index) + 1;
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
    //todo verificator
    free(buffer->buffer);
    fclose(BASE);
    
    return 0;
}

int akinator_guess(Node *node)
{
    char choice = ' ';
    
    Node *tmp_node = node;

    SAY_AND_WRITE("If you agree input \'y\', else input \'n\'");

    while(tmp_node->l_son != NULL)
    {   
        char str [512] = {};
        sprintf(str, "%s?\n", tmp_node->data);
        SAY_AND_WRITE(str);

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
                SAY_AND_WRITE("Why you so stupid?");
                reset_stdin();
            }
        }
    }

    char str [512] = {};
    sprintf(str,"Is it %s?", tmp_node->data);
    SAY_AND_WRITE(str);

    scanf(" %c", &choice);
    
    switch(choice)
        {
            case 'y':
            {
                SAY_AND_WRITE("Its so obvious");
                break;
            }
            case 'n':
            {
                akinator_add_node(tmp_node);
                break;
            }
            default:
            {
                SAY_AND_WRITE("Why you so stupid?");
                reset_stdin();
            }
        }

    // fprintf(BASE, "tralala\n");
    return 0;
}

int akinator_add_node(Node *node)
{   
    reset_stdin();

    char *new_obj = (char*) calloc(LEN_OF_DATA, sizeof(char));
    char *difference = (char*) calloc(LEN_OF_DATA, sizeof(char)); 

    SAY_AND_WRITE("Ok, what it was?\n");
    fgets(new_obj, LEN_OF_DATA, stdin);
    strchr(new_obj, '\n')[0] = '\0';

    char str[512] = {};
    sprintf(str, "What difference between %s and %s?\n", new_obj, node->data);
    SAY_AND_WRITE(str);

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
    if (!node)
    {
        return 0;
    }

    if(node->parent == NULL)
    {
        fseek(BASE, 0, SEEK_SET);   
    }

    fprintf(BASE, "{ #%s", node->data);
    // fprintf(stdout, "{ #%s", node->data);

    
    if(node->l_son)
        {   
            fprintf(BASE, "\n");
            akinator_update_base(node->l_son);
        }

    else    
    {
        fprintf(BASE, "}");
        return 0;
    }

    if(node->r_son)
        {   
            fprintf(BASE, "\n");
            akinator_update_base(node->r_son);
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
    SAY_AND_WRITE("What do you want to define?");

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
        SAY_AND_WRITE("We dont have this object");
    }

    return 0;
}

Node *find_node(Node *node, const char *str)
{   
    if (!node)
        return 0;

    Node *tmp_node = 0;

    // printf("node->date = %s!\n", node->data);
    // printf("str = %s!\n", str);
    // printf("result of comparison = %d\n", strcmp(node->data, str));

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
    if (node->parent)
    {
        print_def(node->parent);
    }

    if ((node->parent) && (node->parent->l_son == node))
    {
        SAY_AND_WRITE(node->parent->data);          //todo make it func and flag to speak
    }

    else if ((node->parent) && (node->parent->r_son == node))
    {   
        char str[LEN_OF_DATA] = {};
        sprintf(str, "not %s", node->parent->data);
        SAY_AND_WRITE(str);
    }

    return 0;
}

int akinator_compare(Node *node)
{   
    char obj_1[LEN_OF_DATA] = {};
    char obj_2[LEN_OF_DATA] = {};
    Node *node1 = {};
    Node *pedigree1[128] = {};

    Node *node2 = {};
    Node *pedigree2[128] = {};

    SAY_AND_WRITE("Input first object for comparison:");
    get_str(obj_1);

    SAY_AND_WRITE("Input second object:");
    get_str(obj_2);

    node1 = find_node(node, obj_1);
    node2 = find_node(node, obj_2);

    if (!(node1 && node2))
    {
        SAY_AND_WRITE("You input wrong obj");
        return 0;
    }

    int depth1 = write_pedigree(pedigree1, node1, 0);
    pedigree1[depth1] = node1;

    // for (int i = 0; i <= depth1; i++)
    //     printf("pedigree1 = %s\n", pedigree1[i]->data); 


    int depth2 = write_pedigree(pedigree2, node2, 0);
    pedigree2[depth2] = node2;

    compare_pedigrees(pedigree1, pedigree2);

    return 0;
}

int write_pedigree(Node *pedigree[], Node *node, int depth)
{
    if (!node)
    {
        // DBG;   
        return 0;
    }
    
    if(node->parent) 
    {   
        // DBG;
        depth = write_pedigree(pedigree, node->parent, depth);
        pedigree[depth] = node->parent;
        // printf("%d -- %s\n", depth, pedigree[depth]->data);
        depth++;
    }

    return depth;
}

int compare_pedigrees(Node *pedigree1[], Node *pedigree2[])
{
    int index = 0;
    char tmp_str[512] ={};
    while((pedigree1[index+1] != NULL) && (pedigree2[index+1] != NULL) && (pedigree1[index] == pedigree2[index]))
    {   
        
        if (pedigree1[index+1] == pedigree2[index + 1])
        {
            
            sprintf(tmp_str, "They both are %s", pedigree1[index]->data);
            
            SAY_AND_WRITE(tmp_str);
        }
        else
        {   
            
            if (pedigree1[index+1]->pos == LEFT)
            {
                        //todo add define                               //todo rename
                sprintf(tmp_str, "But first are %s%s", TEST_ON_ADD_NODE(index+1, pedigree1), pedigree1[index]->data);
                
                SAY_AND_WRITE(tmp_str);
            }
            else
            {   
                
                sprintf(tmp_str, "But second are %s %s", TEST_ON_ADD_NODE(index+1, pedigree2), pedigree2[index]->data);
                
                SAY_AND_WRITE(tmp_str);
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
            sprintf(tmp_str, "first also are %s %s", TEST_ON_ADD_NODE(index1+1, pedigree1), pedigree1[index1]->data);
            SAY_AND_WRITE(tmp_str);
            index1++;
        }
    }
    
    if (pedigree2[index2 + 1] != NULL)
    {
        while(pedigree2[index2+1] != NULL)
        {   
            sprintf(tmp_str, "second also are %s %s", TEST_ON_ADD_NODE(index2+1, pedigree2), pedigree2[index2]->data);
            SAY_AND_WRITE(tmp_str);
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