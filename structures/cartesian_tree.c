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

void split(Node* n, int key, Node** a, Node** b)
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
    Node* new_node = create_node(key, 1 + rand() % 10);
    less = merge(less, new_node);
    root = merge(less, greater);

    return root;
}

void print_inorder_traversal(Node* root, int depth)
{
    if(root)
    {
        print_inorder_traversal(root->left, depth + 1);
        for(int i = 0; i < depth*3; i++)
        {
            printf("   ");
        }
        printf("./(%d)%d)", root->key, root->priority);
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

Node* search(Node* root, int key)
{
    if(!root || root->key == key)
    {
        return root;
    }

    if(key < root->key)
    {
        return search(root->left, key);
    }

    else
    {
        return search(root->right, key);
    }
}

Node* extract(Node* root, int key) 
{
    Node* node_to_remove = search(root, key);
    
    if (!node_to_remove) {
        return root;
    }
    
    Node* less;
    Node* greater;
    split(root, key, &less, &greater);
    root = merge(less, greater);
    free(node_to_remove);

    return root;
}


int main()
{
    Node* root = NULL;

    for(int i = 0; i < 10; i++)
    {
        int key = 1 + rand() % 10;
        int priority = 1 + rand() % 10;

        root = insert(root, key, priority);
        
    }

    printf("cartesian tree\n\n");
    print_inorder_traversal(root, 0);
    printf("\n\n\n\n");
    Node* result = search(root, 5);
    if(result)
    {
        printf("yes\n");
        extract(root, 5);
        print_inorder_traversal(root, 0);

    }
    else
    {
        printf("no\n");
    }
    destroy_tree(root);

    return 0;
}
