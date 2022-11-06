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

int akinator_handle_base(const char* path_to_file, Buffer *buff)
{
    FILE *file_with_buff = fopen(path_to_file, "r");

    if(NULL == file_with_buff)                  //todo akinator verificator
    {
        printf("cant open buffer file");
        return -1;
    }

    buff->size = akinator_size_for_buffer(file_with_buff, path_to_file);

    buff->buffer = (char*) calloc(buff->size, sizeof(char));        //todo verify
    assert(buff->buffer != NULL);

    int test_fread = 0;
    test_fread = fread(buff->buffer, sizeof(char), buff->size, file_with_buff);      //todo verify
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

int akinator_size_for_buffer(FILE *file_with_buff, const char *path_to_file)
{
    struct stat buf ={};
    stat(path_to_file, &buf);

    return buf.st_size + 1;
}


int akinator_do_tree(Node *node, Buffer *buff)
{   
    char new_str[256] = {};
     
    if (buff->curr_index >= buff->size - 1)
    {
        return 0;
    }

    if (strchr(buff->buffer + buff->curr_index, '{'))
        {   
            copy_without_bracket(strchr(buff->buffer + buff->curr_index, '{') + 1, node->data);
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
    char new_str[256] = {};

    if (buff->curr_index >= buff->size - 1)
    {
        return 0;
    }
    
    if (strchr(buff->buffer + buff->curr_index, '{'))
        {   
            Node *l_node = node_ctor_connect(node, LEFT);

            if (strchr(buff->buffer + buff->curr_index, '}'))
            {   
                
                copy_without_bracket(strchr(buff->buffer + buff->curr_index, '{') + 1, l_node->data);
                

                buff->curr_index += strlen(buff->buffer + buff->curr_index) + 1;
            }

            else
            {   
                akinator_do_tree(l_node, buff);
            }

            Node *r_node = node_ctor_connect(node, RIGHT);
            if (strchr(buff->buffer + buff->curr_index, '}'))
            {   
                
                copy_without_bracket(strchr(buff->buffer + buff->curr_index, '{') + 1, r_node->data);
                

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

    return 0;
}

int copy_without_bracket(char* str, char *new_str)
{   
    int i = 0;

    if (strchr(str, '}'))
    {      
        while((str[i] != '}'))
        {   
            new_str[i] = str[i];
            i++;
        }
        
        new_str[i] = '\0';
    }
    else 
    {
        while((str[i] != '\0'))
        {   
            new_str[i] = str[i];
            i++;
            
        }
    }

    return 0;
}

int akinator_guess(Node *node)
{
    char choice = ' ';
    
    Node *tmp_node = node;

    puts("If you agree input \'y\', else input \'n\'");

    while(tmp_node->l_son != NULL)
    {
        printf("%s\n", tmp_node->data);

        scanf(" %c", &choice);

        switch(choice)
        {
            case 'y':
            {
                tmp_node = tmp_node->l_son;
                break;
            }
            case 'n':
                tmp_node = tmp_node->r_son;
                break;
            default:
                puts("Why you so stupid?");
        }
    }

    printf("ну я же говорил, что это  %s\n", tmp_node->data);

    return 0;
}