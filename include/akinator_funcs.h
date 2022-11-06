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

int copy_without_bracket(char* str, char *new_str);

int akinator_guess(Node *node);


