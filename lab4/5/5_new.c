#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

typedef enum
{
    success,
    invalid_input,
    memory_error,
    error_with_opening_file,
    empty = '\0'

}status_code;

typedef struct Stack_elem
{
    char data;
    struct Stack_elem* next;

}Stack_elem;

typedef struct
{
    Stack_elem* top;

}Stack;

void create_stack(Stack* stack)
{
    stack->top = NULL;
}

void push(Stack* stack, char data)
{
    Stack_elem* new_elem = (Stack_elem*)malloc(sizeof(Stack_elem));
    if(!new_elem)
    {
        return;
    }

    new_elem->data = data;
    new_elem->next = stack->top;
    stack->top = new_elem;
}

char pop(Stack** stack)
{
    Stack_elem* out = (*stack)->top;
    char value = out->data;
    (*stack)->top = (*stack)->top->next;
    free(out);

    return value;
}

char peek(Stack* stack)
{
    if(stack->top == NULL)
    {
        return empty;
    }

    return stack->top->data;
}

void destroy_stack(Stack* stack)
{
    Stack_elem* current = stack->top;
    while(current != NULL)
    {
        Stack_elem* next = current->next;
        free(current);
        current = next;
    }
    stack->top = NULL;
}

int is_empty(Stack* stack)
{
    return (stack->top == NULL);
}

int is_operator(const char sym)
{
    return ((sym == '+') || (sym == '-') || (sym == '*') || (sym == '/') || (sym == '^') || (sym == '%') || (sym == '~'));
}

int priotity(const char operator)
{
    switch (operator)
    {
        case '(':
        case ')':
            return 0;

        case '+':
        case '-':
            return 1;

        case '*':
        case '/':
        case '%':
            return 2;

        case '^':
            return 3;

        default:
            return 4;
    }
}

status_code get_reverse_poish(const char* infix, char* postfix)
{
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    if(!stack)
    {
        return memory_error;
    }

    create_stack(stack);
    
    int idx_inf = 0, idx_post = 0;

    while(infix[idx_inf] != '\0')
    {
        if(isdigit(infix[idx_inf]))
        {
            while(isdigit(infix[idx_inf]))
            {
                postfix[idx_post++] = infix[idx_inf++];
            }
            postfix[idx_post++] = ' ';
            idx_inf--;
        }
        else if(infix[idx_inf] == '(')
        {
            push(stack, infix[idx_inf]);
        }
        else if(infix[idx_inf] == ')')
        {
            while(!is_empty(stack) && peek(stack) != '(')
            {
                postfix[idx_post++] = pop(&stack);
                postfix[idx_post++] = ' ';
                pop(&stack);
            }
            if(!is_empty(stack) && peek(stack) == '(')
            {
                pop(&stack);
            }
        }
        else if(is_operator(infix[idx_inf]))
        {
            if(infix[idx_inf] == '-' && (idx_inf == 0 || is_operator(infix[idx_inf - 1]) || infix[idx_inf - 1] == '('))
            {
                postfix[idx_post++] = '0';
                postfix[idx_post++] = ' ';
                push(stack, '-');
            }
            else
            {
                while(!is_empty(stack) && priotity(peek(stack)) >= priotity(infix[idx_inf]))
                {
                    postfix[idx_post++] = peek(stack);
                    postfix[idx_post++] = ' ';
                    pop(&stack);
                }
                push(stack, infix[idx_inf]);
            }
        }
        idx_inf++;
    }
    while(!is_empty(stack))
    {
        postfix[idx_post++] = peek(stack);
        pop(&stack);
    }
    postfix[idx_post] = '\0';
    destroy_stack(stack); 
    return success;
}


char* create_error_file(char* filename)
{
    char* output_file = NULL;
    output_file = (char*)malloc(strlen(filename) + 8); //errors_
    if(!output_file)
    {
        return NULL;
    }
    strcpy(output_file, "errors_");
    strcat(output_file, filename);

    return output_file;
}

status_code proccess_bracket_epression(FILE* input_file, FILE* output_file)
{
    char sym;
    char string[256];
    int flag = 1;
    int line = 0;
    int index = 0;
    int brackets_counter = 0;

    while(flag)
    {
        int index = 0;
        int brackets_counter = 0;
        //check brackets balance
        while((sym = getc(input_file)) != EOF && sym != '\n')
        {
            string[index] = sym;
            index++;

            if(sym == '(')
            {
                brackets_counter++;
            }
            else if(sym == ')')
            {
                brackets_counter--;
            }
        }
        string[index] = '\0';
        printf("infix: %s\n", string);
        

        if(brackets_counter != 0)
        {
            fprintf(output_file,"infix: %s is not balanced with brackets in line: %d\n",string, line);
        }
        else
        {
            printf("The infix: %s is balanced by brackets!\n", string);
            char polish[250];
            status_code status = get_reverse_poish(string, polish);
            if(status == success)
            {
                printf("polish expression: %s\n", polish);
            }
        }
        if(sym == EOF)
        {
            flag = 0;
        }
        line++;

    }

    return success;
}

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        printf("invalid input\n");
        return invalid_input;
    }

    FILE* input_file = NULL;
    char* out = NULL;
    FILE* output_file = NULL;

    for(int i = 1; i < argc; i++)
    {
        input_file = fopen(argv[i], "r");
        if(!input_file)
        {
            printf("error with opening file\n");
            return error_with_opening_file;
        }

        printf("file: %s\n", argv[i]);
        
        out = create_error_file(argv[i]);
        output_file = fopen(out, "a");
        if(!output_file)
        {
            printf("error with opening output file\n");
            fclose(input_file);
            return error_with_opening_file;
        }

        status_code proccess_status = proccess_bracket_epression(input_file, output_file);
        switch(proccess_status)
        {
            case success:
                printf("everything ok\n");
                break;
        }

        fclose(input_file);
        fclose(output_file);
    }

    
    return 0;
}