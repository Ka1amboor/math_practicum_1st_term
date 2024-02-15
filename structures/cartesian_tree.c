#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Node
{
    int key;
    int priority;
    struct Node* left;
    struct Node* right;

}Node;

Node* create_node(int data, int priority)
{
    Node* new_node = (Node*)malloc(sizeof(Node));
    if(!new_node)
    {
        return NULL;
    }

    new_node->left = NULL;
    new_node->right = NULL;
    new_node->priority = priority;
    new_node->key = data;

    return new_node;
}

Node* merge(Node* a, Node* b)
{
    if(!a || !b)
    {
        return a ? a : b;
    }

    else if(a->priority > b->priority)
    {
        a->right = merge(a->right, b);
        return a;
    }

    else
    {
        b->left = merge(a, b->left);
        return b;
    }
}

static void split(Node* n, int key, Node** a, Node** b)
{
    if(!n)
    {
        *a = *b = NULL;
        return;
    }

    else if(n->key <= key)
    {
        split(n->right, key, &(n->right), b);
        *a = n;
    }

    else
    {
        split(n->left, key, a, &(n->left));
        *b = n;
    }
}

Node* insert(Node* root, int key, int priority)
{
    Node* less; //for slit all keys < key
    Node* greater;// all keys > key

    split(root, key, &less, &greater);
    Node* new_node = create_node(key, rand());
    less = merge(less, new_node);
    root = merge(less, greater);

    return root;
}

void print_inorder_traversal(Node* root, int depth)
{
    if(root)
    {
        print_inorder_traversal(root->left, depth + 1);
        for(int i = 0; i < depth; i++)
            printf("\t");
        printf("(%d)%d)", root->key, root->priority);
        print_inorder_traversal(root->right, depth + 1);
    }
}

void destroy_tree(Node* root)
{
    if(root)
    {
        destroy_tree(root->left);
        destroy_tree(root->right);
        free(root);
    }
}

int main()
{
    Node* root = NULL;

    for(int i = 0; i < 10; i++)
    {
        int key = 80 + rand() % 21;
        int priority = 80 + rand() % 21;

        root = insert(root, key, priority);
        
    }

    printf("cartesian tree\n\n");
    print_inorder_traversal(root, 0);
    destroy_tree(root);

    return 0;
}
