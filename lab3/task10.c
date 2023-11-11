#include <stdio.h>
#include <stdlib.h>

enum status_code
{
    SUCCESS = 0,
    INVALID_INPUT = 1,
    MEMORY_ERROR = 2,
    FILE_ERROR = 3
};

typedef struct Node
{
    char data;
    struct Node* parent;
    struct Node* son;
    struct Node* brother;

} Node;


void create_node(Node* new_node, char data)
{
    
    new_node->data = data;
    new_node->parent = NULL;
    new_node->son = NULL;
    new_node->brother = NULL;
    
}

enum status_code add_son(Node* parent, char data)
{
    Node* child = malloc(sizeof(Node));
    if (!child)
    {
        return MEMORY_ERROR;
    }
    child->parent = parent;
    child->brother = NULL;
    child->son = NULL;
    child->data = data;
    if (parent->son == NULL)
    {
        parent->son = child;
    }
    else
    {
        Node* current = parent->son;
        while (current->brother != NULL)
        {
            current = current->brother;
        }
        current->brother = child;
    }
    return SUCCESS;
}

enum status_code add_brother(Node* node, char data)
{
    Node* bro = malloc(sizeof(Node));
    if (!bro)
    {
        return MEMORY_ERROR;
    }
    bro->data = data;
    bro->parent = node->parent;
    bro->son = NULL;
    bro->brother = NULL;
    if (node->brother == NULL)
    {
        node->brother = bro;
    }
    else
    {
        Node* current = node->brother;
        while (current->brother != NULL)
        {
            current = current->brother;
        }
        current->brother = bro;
    }
    return SUCCESS;
}

void print_tree(Node* root, int level, FILE* file)
{
    if (root == NULL)
    {
        return;
    }
    for (int i = 0; i < level; i++)
    {
        fprintf(file, "%s", "  ");
    }
    fprintf(file, "%c\n", root->data);
    print_tree(root->son, level + 1, file);
    print_tree(root->brother, level, file);
}

enum status_code process_bracket_expression(char* expression, FILE* file)
{
    Node* root = malloc(sizeof(Node));
    if (!root)
    {
        return MEMORY_ERROR;
    }
    create_node(root, expression[0]);
    Node* current = root;
    for (int i = 1; expression[i] != '\0'; i++)
    {

        if (expression[i] == '(')
        {
            Node* new_node = malloc(sizeof(Node));
            if (!new_node)
            {
                return MEMORY_ERROR;
            }
            create_node(new_node, expression[i + 1]);
            if (root == NULL)
            {
                root = new_node;
                current = new_node;
            }
            else
            {
                add_son(current, expression[i+1]);
                current = current->son;
            }
        }
        else if (expression[i] == ',')
        {
            add_brother(current, expression[i + 1]);
            Node* temp = current;
            while (temp->brother != NULL)
            {
                temp = temp->brother;
            }
            current = temp;
        }
        else if (expression[i] == ')')
        {
            current = current->parent;

        }

    }

    print_tree(root, 0, file);
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        printf("invalid value");
        return INVALID_INPUT;
    }

    FILE* input_file = fopen(argv[1], "r");
    if (!input_file)
    {
        printf("Error opening file\n");
        return FILE_ERROR;
    }
    FILE* output_file = fopen(argv[2], "w");
    if (!output_file)
    {
        printf("Error opening file\n");
        fclose(input_file);
        return FILE_ERROR;
    }

    char expression[100];

    while (fgets(expression, 100, input_file) != NULL)
    {
        process_bracket_expression(expression, output_file);
        
    }

    fclose(input_file);
    fclose(output_file);

    return 0;
}
