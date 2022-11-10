#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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

    return node;
}

Node *node_ctor_connect(Node *parent, Pos_of_node pos)
{
    Node *node = node_ctor();

    node_connect(parent, node, pos);

    return node;
}

Node* node_connect(Node *parent, Node *node, Pos_of_node pos)
{
    //todo checker on rewrite of parent node

    switch (pos)       
    {
        case LEFT:
        {   
            
            parent->l_son = node;
            node->pos = LEFT;
            
            break;
        }
        case RIGHT:
        {
            parent->r_son = node;
            node->pos = RIGHT;

            break;
        }

        default:
            printf("You make a mistake in input of pos\n");
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
    
    node->data = NULL;
    free(node);
    node = NULL;

    return 0;
}

int node_dtor_calloc_data(Node *node, const char* buffer, int size)
{
    if (node->l_son != NULL)
    {
        node_dtor_calloc_data(node->l_son, buffer, size);
    }

    if (node->r_son != NULL)                                        //todo 1 traverse on tree
    {
        node_dtor_calloc_data(node->r_son, buffer, size);
    }

    if (fabs(buffer - (node->data)) > size)
    {
        free(node->data);
    }

    return 0;
}