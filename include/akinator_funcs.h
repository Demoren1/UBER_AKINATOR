typedef struct Buffer_t
{
    char *buffer;
    size_t size;
    int curr_index;
} Buffer;

int akinator_handle_base(const char* path_to_file, Buffer *buff);

int akinator_size_for_buffer(FILE *file_with_buff, const char *path_to_file);

int akinator_dtor(Buffer *buff);

int akinator_do_tree(Node *node, Buffer *buff);

int link_nodes(Node *node, Buffer *buff);

int akinator_guess(Node *node);

int akinator_add_node(Node *node);

int akinator_update_base(Node *node);

int reset_stdin();

int akinator_define(Node *node);

Node* find_node(Node *node, const char *str);

enum Regimes
{
    GUESS       = 1,
    GIVE_DEF    = 2,
    COMPARE     = 3,
    GRAPH_DUMP  = 4,
    QUIT        = 5
};
