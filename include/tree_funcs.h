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
    char *data;
    Node_t* parent;
    Node_t* l_son;
    Node_t* r_son;
    Pos_of_node pos;

} Node;

typedef struct Tree_t
{
    Node* root;
} Tree;

int tree_ctor(Tree *tree);

[[nodiscard]]Node *node_ctor();

Node* node_connect(Node *parent, Node *node, Pos_of_node pos);

Node* node_ctor_connect(Node *parent, Pos_of_node pos);

int node_dtor(Node *node);

int node_dtor_calloc_data(Node *node, const char* buffer, int size);

int tree_show_graph();




