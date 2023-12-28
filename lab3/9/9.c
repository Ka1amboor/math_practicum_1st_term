#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef enum
{
    SUCCESS,
    INVALID_INPUT,
    FILE_ERROR,
    MEMORY_ERROR,
    ERROR

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

void read_words_and_insert_nodes(FILE* file, Node** root, const char* separators)
{
    char buffer[100];
    while (fscanf(file, "%s", buffer) != EOF) 
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
    for (int i = 0; i < depth*3; i++)
    {
        printf("  ");
    }
    printf("./%s(%d)\n", root->word, root->count);
    print_tree(root->right, depth + 1);
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
    printf("*  Enter <3> for search the longest     *\n");
    printf("*        and shortest word              *\n");
    printf("*                                       *\n");
    printf("****************************************\n");
}

int count_node(Node* root)
{
    if(root == NULL)
    {
        return 0;
    }

    return 1 + count_node(root->left) + count_node(root->right);
}

void count_word_occurrences(Node* node, char* word, int* times_1)
{
    if(node == NULL)
    {
        return;
    }
    int res_strcmp = strcmp(node->word, word);

    if(res_strcmp == 0)
    {
        (*times_1)++;
    }
    else if(res_strcmp < 0)
    {
        count_word_occurrences(node->left, word, times_1);
    }
    else
    {
        count_word_occurrences(node->right, word, times_1);
    }
}

int compare(const void* a, const void* b)
{
    Node* node_a = *(Node**)a;
    Node* node_b = *(Node**)b;

    return node_b->count - node_a->count;
}

status_code all_nodes(Node* node, Node*** array_of_nodes, int* index_2) //in_order
{
    status_code status = SUCCESS;

    if(node == NULL)
    {
        return SUCCESS;
    }

    status = all_nodes(node->left, array_of_nodes, index_2);
    if(status != SUCCESS)
    {
        return status;
    }

    (*array_of_nodes)[*index_2] = node;
    (*index_2)++;

    status = all_nodes(node->right, array_of_nodes, index_2);
    if(status != SUCCESS)
    {
        return status;
    }

    return SUCCESS;
}

void search_min_and_max_word(Node* root, Node** min_3, Node** max_3)
{
    if(root == NULL)
    {
        return;
    }
    search_min_and_max_word(root->right, min_3, max_3);
    if(strlen(root->word) > strlen((*max_3)->word))
    {
        (*max_3) = root;
    }
    if(strlen(root->word) < strlen((*min_3)->word))
    {
        *min_3 = root;
    }

    search_min_and_max_word(root->left, min_3, max_3);

}

void get_depth(Node* node, unsigned int* max_depth_3, int depth_3)
{
    if(node == NULL)
    {
        return;
    }
    if(node->left)
    {
        get_depth(node->left, max_depth_3, depth_3 + 1);
    }
    if(node->right)
    {
        get_depth(node->right, max_depth_3, depth_3 + 1);
    }
    if(depth_3 > (*max_depth_3))
    {
        (*max_depth_3) = depth_3;
    }
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

    read_words_and_insert_nodes(file_input, &root, separators);
    int count_of_nodes = count_node(root);
    printf("The number of nodes: %d\n",count_of_nodes);
    free(separators);
    int flag = 1;

    while(flag)
    {   
        int action = 0;
        char word_for_find_1[100];
        int count_word_1 = 0;
        int times_1 = 0;
        int n_2 = 0;
        int index_2 = 0;
        Node* min_3 = root;
        Node* max_3 = root;
        int depth_3 = 1;
        unsigned int max_depth_3 = 0;
        print_tree(root, 0);
        print_options();
        scanf("%d", &action);

        if(check_action(action) == INVALID_INPUT)
        {
            printf("invalid input\n");
            root = destroy_tree(root);
            return INVALID_INPUT;
        }

        if (action == 0)
        {
            flag = 0;
            root = destroy_tree(root);
            break;
        }

        switch(action)
        {
            case 1:
                printf("Please enter word wich you want to find:\n");
                scanf("%s", &word_for_find_1);
                count_word_occurrences(root, word_for_find_1, &times_1);
                printf("Word '%s occurred %d times in the file.\n", word_for_find_1, times_1);
                break;

            case 2:
                printf("please enter <n>:\n");
                if(scanf("%d", &n_2) != 1)
                {
                    printf("invalid number\n");
                    root = destroy_tree(root);
                    return INVALID_INPUT;
                }

                Node** array_of_nodes = (Node**)malloc(sizeof(Node*) * count_of_nodes);
                if(!array_of_nodes)
                {
                    destroy_tree(root);
                    printf("failed toallocate memory\n");
                    return MEMORY_ERROR;
                }

                if(all_nodes(root, &array_of_nodes, &index_2) != SUCCESS)
                {
                    destroy_tree(root);
                    free(array_of_nodes);
                    printf("error\n");
                    return ERROR;
                }

                for(int i = 0; i < count_of_nodes; i++)
                {
                    printf("%s\n", array_of_nodes[i]->word);
                }
                printf("\n");
                qsort(array_of_nodes, count_of_nodes, sizeof(Node**), compare);
                for(int i = 0; i < n_2; i++)
                {
                    printf("%s\n", array_of_nodes[i]->word);
                }
                printf("\n");
                free(array_of_nodes);
                break;

            case 3:
                if(root == NULL)
                {
                    printf("tree is empty\n");
                    break;
                }
                search_min_and_max_word(root, &min_3, &max_3);
                printf("min word: %s\n", min_3->word);
                printf("max word: %s\n", max_3->word);
                break;

            case 4:
                if(root == NULL)
                {
                    printf("tree is empty\n");
                    break;
                }
                get_depth(root, &max_depth_3, depth_3);
                printf("max depth of the tree: %u\n", max_depth_3);
                break;
        }
    }
    
    return 0;

}
