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

status_code check_action(int action)
{
    if(action < 0 || action > 9)
    {
        return INVALID_INPUT;
    }
    return SUCCESS;
}

typedef struct Node
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

    strcpy(new_node->word, word);
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

void read_words_and_insert_nodes(FILE* file, Node** root, const char* separators, int* count_all_words)
{
    char buffer[100];
    while (fscanf(file, "%s", buffer) != EOF) 
    {
        char* word = strtok(buffer, separators);
        while (word != NULL)
        {
            *root = insert_node(*root, word);
            (*count_all_words)++;
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
    for (int i = 0; i < depth*3; i++)
    {
        printf("  ");
    }
    printf("./%s(%d)\n", root->word, root->count);
    print_tree(root->right, depth + 1);
}

Node* destroy_tree(Node* root)
{
    if(root)
    {
        destroy_tree(root->left);
        destroy_tree(root->right);
        free(root->word);
        free(root);
    }

    return NULL;
}

void print_options()
{
    printf("****************************************\n");
    printf("*     Please select an action           *\n");
    printf("*                                       *\n");
    printf("*   Enter <0> if you want to exit       *\n");
    printf("*  Enter <1> if you want to see how     *\n");
    printf("*  many times a word occurs in file     *\n");
    printf("* Enter <2> for output of the first     *\n");
    printf("*  n most common words in the file      *\n");
    printf("*                                       *\n");
    printf("****************************************\n");
}

int count_word_occurrences(Node* node, char* word)
{
    if(node == NULL)
    {
        return 0;
    }

    int res_strcmp = strcmp(node->word, word);

    if(res_strcmp == 0)
    {
        return node->count;
    }

    else if(res_strcmp < 0)
    {
        return count_word_occurrences(node->left, word);
    }

    else
    {
        return count_word_occurrences(node->right, word);
    }
}

status_code all_nodes(Node* node, Node*** nodes, int* len_2, int* capacity_2)
{
    status_code status = SUCCESS;
    printf("4\n");
    if(node == NULL)
    {
        return SUCCESS;
    }

    status = all_nodes(node->left, nodes, len_2, capacity_2);
    if(status == MEMORY_ERROR)
    {
        printf("2\n");
        return MEMORY_ERROR;
    }

    if(*len_2 == *capacity_2)
    {
        *capacity_2 *= 2;
        Node** tmp = (Node**)realloc(*nodes, *capacity_2 * sizeof(Node*));
        if(tmp == NULL)
        {   
            printf("1\n");
            return MEMORY_ERROR;
        }
        *nodes = tmp;
    }

    (*nodes)[*len_2] = node;
    (*len_2)++;

    status = all_nodes(node->right, nodes, len_2, capacity_2);
    if(status == MEMORY_ERROR)
    {
        printf("3\n");
        return MEMORY_ERROR;
    }

    return status;
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
    if (check_input(argc, argv) == INVALID_INPUT) 
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
    printf("separators: %s\n", separators);

    Node* root = NULL;
    int count_all_words = 0;

    read_words_and_insert_nodes(file_input, &root, separators, &count_all_words);
    free(separators);

    while(1)
    {
        int action = 0;
        char word_for_find_1[100];
        int count_word_1 = 0;
        int times_1 = 0;
        int n_2 = 0;
        int len_2 = 0;
        int capacity_2 = 0;
        // print_tree(root, 0);
        print_options();
        scanf("%d", &action);

        if(check_action(action) == INVALID_INPUT)
        {
            printf("invalid input\n");
            root = destroy_tree(root);
            return INVALID_INPUT;
        }

        if(action == 0)
        {
            break;
        }
        switch(action)
        {
            case 1:

                if(root == NULL)
                {
                    printf("The tree is empty\n");
                    break;
                }
                printf("Please enter word wich you want to find:\n");
                scanf("%s", &word_for_find_1);
                count_word_1 = count_word_occurrences(root, word_for_find_1);
                printf("Word %s occurred %d times in the file.\n", word_for_find_1, count_word_1);
                break;
            
            case 2:

                if(root == NULL)
                {
                    printf("The tree is empty\n");
                    break;
                }
                printf("Please enter <n>:\n");
                if(scanf("%d", &n_2) != 1)
                {
                    printf("invalid input\n");
                    return INVALID_INPUT;
                }
                printf("lalalal2\n");
                Node** array = (Node**)malloc(sizeof(Node*) * capacity_2);
                if(array == NULL)
                {
                    printf("lalalal3\n");
                    destroy_tree(root);
                    printf("failed to allocate memory\n");
                    return MEMORY_ERROR;
                }
                printf("lalala4\n");
                if(all_nodes(root, &array, &len_2, &capacity_2) != SUCCESS)
                {   
                    printf("lalalal5\n");
                    free(array);
                    destroy_tree(root);
                    printf("failed to allocate memory_2\n");
                    return MEMORY_ERROR;
                }
                printf("lalala6");

                printf("%d", len_2);

                for (int i = 0; i < len_2; i++)
                {
                    printf("%s", array[i]->word);

                }
                printf("\n");
        }
    }

    if(separators) free(separators);
    if(file_input) fclose(file_input);
    destroy_tree(root);
    return 0;

}
