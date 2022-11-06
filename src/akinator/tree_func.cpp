#include <stdio.h>
#include <stdlib.h>
#include <tree_funcs.h>
#include <tree_debug.h>

int tree_ctor(Tree *tree)
{
    tree->root = node_ctor();

    return 0;
}

Node* node_ctor()
{
    //todo add check for calloc

    Node *node = (Node*) calloc(1, sizeof(Node));

    node->data = (char *) calloc(256, sizeof(char));
    return node;
}

Node *node_ctor_connect(Node *parent, Dest_of_node dest)
{
    Node *node = node_ctor();

    node_connect(parent, node, dest);

    return node;
}

Node* node_connect(Node *parent, Node *node, Dest_of_node dest)
{
    //todo checker on rewrite of parent node

    switch (dest)       
    {
    case LEFT:
    {   
        
        parent->l_son = node;
        
        break;
    }
    case RIGHT:
    {
        parent->r_son = node;
        break;
    }

    default:
        printf("You make a mistake in input of dest\n");
        break;
    }
    
    node->parent = parent;
    
    return node;
}

int node_dtor(Node* node)
{
    //todo maybe poison the data

    if (node->l_son != NULL)
    {
        node_dtor(node->l_son);
        node->l_son = NULL;
    }

    if (node->r_son != NULL)
    {
        node_dtor(node->r_son);
        node->r_son = NULL;
    }
    
    free(node->data);
    node->data = NULL;
    free(node);
    node = NULL;

    return 0;
}


