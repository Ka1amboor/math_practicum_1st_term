#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef enum
{
    SUCCESS,
    INVALID_INPUT,
    FILE_ERROR,
    MEMORY_ERROR

} status_code;

typedef struct Node //исправлена ошибка
{
    char* word;
    int count; //how many times we met word
    struct Node* left;
    struct Node* right;

} Node;

Node* create_new_node(char* word)
{
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (!new_node)
    {
        return NULL;
    }
    new_node->word = (char*)malloc((strlen(word) + 1) * sizeof(char));

    if (!new_node->word)
    {
        free(new_node);
        return NULL;
    }

    strcpy(new_node->word, word); //add word
    new_node->count = 1;
    new_node->left = NULL;
    new_node->right = NULL;

    return new_node;

}

Node* insert_node(Node* root, char* word)
{
    if (root == NULL)
    {
        return create_new_node(word);
    }

    int res_strcmp = strcmp(word, root->word);

    if (res_strcmp == 0)
    {
        root->count++;
    }
    else if (res_strcmp < 0)
    {
        root->left = insert_node(root->left, word);
    }
    else
    {
        root->right = insert_node(root->right, word);
    }

    return root;
}

void read_words_and_insert_nodes(FILE* file, Node** root, const char* separators)
{
    char buffer[100];
    while (fscanf(file, "%s", buffer) != EOF) //изменил переменную word на buffer, чтобы не перекрыть переменную word в strtok
    {
        char* word = strtok(buffer, separators);
        while (word != NULL)
        {
            *root = insert_node(*root, word);
            word = strtok(NULL, separators);
        }
    }
}

void print_tree(Node* root, int depth)
{
    if (root == NULL)
    {
        return;
    }

    print_tree(root->left, depth + 1);
    for (int i = 0; i < depth; i++)
    {
        printf("  ");
    }
    printf("%s (%d)\n", root->word, root->count);
    print_tree(root->right, depth + 1);
}

status_code check_input(int argc, const char* argv[])
{
    if (argc < 3) return INVALID_INPUT;

    for (int i = 2; i < argc; i++)
    {
        if (strlen(argv[i]) != 1) return INVALID_INPUT; //separator is a 1 symbol
    }
    return SUCCESS;
}

int main(int argc, const char* argv[]) // argv[1] - input_file others argv[2] argv[3] etc separators
{
    if (check_input(argc, argv) == INVALID_INPUT) //убрана лишняя точка с запятой после условия if
    {
        printf("invalid input\n");
        return INVALID_INPUT;
    }

    FILE* file_input = fopen(argv[1], "r");
    if (!file_input)
    {
        printf("error with opening file\n");
        return FILE_ERROR;
    }

    int lenth_sep = argc - 2;
    char* separators = (char*)malloc(sizeof(char) * (lenth_sep + 1));

    if (!separators)
    {
        printf("failed to allocate memory\n");
        fclose(file_input);
        return MEMORY_ERROR;
    }
    separators[0] = '\0';

    for (int i = 2; i < argc; i++)
    {
        strcat(separators, argv[i]);
    }
    printf("separators: %s\n", separators); //add separators

    Node* root = NULL;

    read_words_and_insert_nodes(file_input, &root, separators);

    free(separators);
    print_tree(root, 0);

    return 0;

}
