#include <stdio.h>
#include <string.h>
#include <tree_funcs.h>
#include <tree_debug.h>
#include <akinator_funcs.h>

int main(int argv, char *argc[])
{   
    open_tree_logs();
    
    Tree tree = {};
    tree_ctor(&tree);
    
    const char *file_path = "test_base.txt"; //todo use argc
    Buffer buff ={};

    akinator_handle_base(file_path, &buff);
    akinator_do_tree(tree.root, &buff);    

    int choice = 0;

    while (choice != 5)
    {   
        printf("Choose, what do you want\n");
        printf("--------------------------------------------------------\n");
        printf("1.Guess                    2.Give definition            \n");
        printf("3.Compare                  4.Graph dump                 \n");
        printf("5.Quit                                                  \n");
        printf("--------------------------------------------------------\n");
        scanf(" %d", &choice);

        switch(choice)
        {
            case GUESS:
            {
                akinator_guess(tree.root);
                break;
            }
            case GIVE_DEF:
            {   
                reset_stdin();
                akinator_define(tree.root);
                break;
            }
            case COMPARE:
            {
                break;
            }
            case GRAPH_DUMP:
            {
                TREE_DUMP(tree.root, INORDER);
                tree_show_graph();
                break;
            }
            case QUIT:
            {
                puts("Bye");
                break;
            }
            default:
            {
                puts("Can you read?");
                reset_stdin();
                break;
            }
        }       

        akinator_update_base(tree.root);
    }

    node_dtor_calloc_data(tree.root, buff.buffer, buff.size);
    node_dtor(tree.root);
    akinator_dtor(&buff);
    close_tree_logs();
    return 0;
}