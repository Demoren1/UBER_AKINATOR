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
        buff->curr_index += strlen(buff->buffer + buff->curr_index) + 1;  
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
                strchr(buff->buffer + buff->curr_index, '}')[0] = ' ';
                l_node->data = strchr(buff->buffer + buff->curr_index, '#') + 1;
                
                buff->curr_index += strlen(buff->buffer + buff->curr_index) + 1;
            }

            else
            {   
                akinator_do_tree(l_node, buff);
            }

            Node *r_node = node_ctor_connect(node, RIGHT);
            if (strchr(buff->buffer + buff->curr_index, '}'))
            {   
                strchr(buff->buffer + buff->curr_index, '}')[0] = ' ';

                r_node->data = strchr(buff->buffer + buff->curr_index, '#') + 1;

                buff->curr_index += strlen(buff->buffer + buff->curr_index) + 1;
                
                while ((buff->curr_index <= buff->size -1) &&(strchr(buff->buffer + buff->curr_index, '{') == NULL))
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

    free(buffer->buffer);
    fclose(BASE);
    
    return 0;
}

int akinator_guess(Node *node)
{
    char choice = ' ';
    
    Node *tmp_node = node;

    puts("If you agree input \'y\', else input \'n\'");

    while(tmp_node->l_son != NULL)
    {
        printf("%s?\n", tmp_node->data);

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
                puts("Why you so stupid?");
                reset_stdin();
            }
        }
    }

    printf("Is it %s?\n", tmp_node->data);
    scanf(" %c", &choice);
    
    switch(choice)
        {
            case 'y':
            {
                printf("Its so obvious\n");
                break;
            }
            case 'n':
            {
                akinator_add_node(tmp_node);
                break;
            }
            default:
            {
                puts("Why you so stupid?");
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

    printf("Ok, what it was?\n");
    fgets(new_obj, LEN_OF_DATA, stdin);
    strchr(new_obj, '\n')[0] = '\0';

    printf("What difference between %s and %s?\n", new_obj, node->data);
    fgets(difference, LEN_OF_DATA, stdin);
    strchr(difference, '\n')[0] = '\0';

    
    Node *l_node = node_ctor_connect(node, LEFT);
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

    fprintf(BASE, "{ #%s ", node->data);
    
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
    
    fprintf(BASE," \n}");

    return 0;
}

int akinator_define(Node *node)
{
    puts("What do you want to define?");

    char str[LEN_OF_DATA] = {};
    fgets(str, LEN_OF_DATA, stdin);

    Node *proper_node = find_node(node, str);

    return 0;
}

Node *find_node(Node *node, const char *str)
{   
    if (!node)
        return 0;

    Node *tmp_node = 0;
    printf("node data  = %s\n", node->data);
    printf("str  = %s\n", str);

    if (strcmp(node->data, str) == 0)
    {   
        printf("Its find\n");
        return node;
    }

    else if (node->r_son)
    {
        tmp_node = find_node(node->r_son, str);
    }

    else if ((node->r_son) && (tmp_node == NULL))
    {
        tmp_node = find_node(node->r_son, str);
    }

    return tmp_node;
}