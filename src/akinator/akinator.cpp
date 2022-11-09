#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
        SAY_AND_WRITE("Choose, what do you want");
        SAY_AND_WRITE("--------------------------------------------------------");
        SAY_AND_WRITE("1.Guess                    2.Give definition            ");
        SAY_AND_WRITE("3.Compare                  4.Graph dump                 ");
        SAY_AND_WRITE("5.Quit                                                  ");
        SAY_AND_WRITE("--------------------------------------------------------");
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
                reset_stdin();
                akinator_compare(tree.root);
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
                SAY_AND_WRITE("Bye");
                break;
            }
            default:
            {
                SAY_AND_WRITE("Can you read?");
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