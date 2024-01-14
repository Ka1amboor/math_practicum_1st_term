#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum
{
    success,
    memory_error,
    file_error,
    invalid_input,
    empty

}status_code;

typedef struct Node
{
    char data;
    struct Node* next;

}Node;

typedef struct
{
    Node* top;

}Stack;

void create_stack(Stack* stack)
{
    stack->top = NULL;
}

status_code push(Stack* stack, char data)
{
    Node* new_node = (Node*)malloc(sizeof(Node));
    if(!new_node) return memory_error;
    
    new_node->data = data;
    new_node->next = stack->top;
    stack->top = new_node;

    return success;
}

char pop(Stack* stack)
{
    Node* tmp = stack->top;
    char data = tmp->data;
    stack->top = tmp->next;
    free(tmp);
    return data;
}

int is_empty(Stack* stack)
{
    return (stack->top == NULL) ? 1 : 0;
}

void destroy_stack(Stack* stack)
{
    Node* current = stack->top;
    while(current != NULL)
    {
        Node* tmp = current;
        current = current->next;
        char data = current->data;
        free(tmp);
    }
    stack->top = NULL;
}

int check_brackets_balance(char* expression)
{
    Stack stack;
    stack.top = NULL;

    int len = strlen(expression);
    for(int i = 0; i < len; i++)
    {
        char sym = expression[i];
        if(sym == '(') push(&stack, sym);
        else if(sym == ')')
        {
            if(is_empty(&stack)) return 0; //ne hvataet otcryvayushih;
            else
            {
                if(pop(&stack) != '(') return 0;

            }
        }
    }
    if(is_empty(&stack)) return 1;
    else return 0;
    destroy_stack(&stack);
}

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        printf("invalid input!\n please enter <file_name_1.txt>...<file_mame_n.txt\n");
        return invalid_input;
    }

    int flag = 1;
    int number_of_expression = 0;
    char string[1000];
    FILE* file;

    for(int i = 1; i < argc; i++)
    {
        file = fopen(argv[i], "r"); //prohodimsya po files
        if(!file)
        {
            printf("failed to opening file\n");
            return file_error;
        }
        printf("%s file: \n", argv[i]);
    }

    while(fgets(string, sizeof(string), file))
    {
        printf("expression before: %s\n", string);
        number_of_expression++;
        char expression[1000];
        strcpy(expression, string);
        if(check_brackets_balance(expression))
        {
            printf("ok\n");
        }
        else
        {
            printf("not ok\n");
        }

    }
    number_of_expression = 0; //next file
    return 0;
}
