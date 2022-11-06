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

    // TREE_DUMP(tree.root, INORDER);
    
    akinator_guess(tree.root);
    
    TREE_DUMP(tree.root, INORDER);

    node_dtor(tree.root);

    akinator_dtor(&buff);
    close_tree_logs();
    return 0;
}