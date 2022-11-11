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

    akinator_voice();
    akinator_handle_base(file_path, &buff);
    akinator_do_tree(tree.root, &buff);    

    int choice = 0;

    while (choice != 5)
    {      
        //todo in 1 func
        say_and_write("\nChoose, what do you want");
        say_and_write("--------------------------------------------------------");
        say_and_write("1.Guess                    2.Give definition            ");
        say_and_write("3.Compare                  4.Graph dump                 ");
        say_and_write("5.Quit                                                  ");
        say_and_write("--------------------------------------------------------");
        scanf(" %d", &choice);
        //todo reset in end of while 

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
                say_and_write("Bye");
                break;
            }
            default:
            {
                say_and_write("Can you read?");
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