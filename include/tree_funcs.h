#ifndef TREE_GUARD
#define TREE_GUARD

const int LEN_OF_DATA = 256;

#define FUNC_GENERAL_INFO(object)  __FILE__, __FUNCTION__, #object, __LINE__

#define TREE_CTOR(tree) tree_ctor(&list, FUNC_GENERAL_INFO(tree))

#define POISON "POISON"

typedef enum 
{
    LEFT = 1,
    RIGHT = 2
}Pos_of_node;

typedef struct Node_t
{
    char *data      = NULL;
    Node_t* parent  = NULL;
    Node_t* l_son   = NULL;
    Node_t* r_son   = NULL;
    Pos_of_node pos;

} Node;

typedef struct Tree_t
{
    Node* root;
} Tree;

int tree_ctor(Tree *tree);

Node *node_ctor(bool flag_on_data);

Node* node_connect(Node *parent, Node *node, Pos_of_node pos);

Node* node_ctor_connect(Node *parent, Pos_of_node pos);

int node_dtor(Node *node);

int node_dtor_calloc_data(Node *node, const char* buffer, int size);

int tree_show_graph();
#endif



