#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>
typedef enum
{
    success,
    invalid_input,
    memory_error,
    error_with_opening_file,
    empty = '\0',
    division_by_zero,
    uncertainty,
    overflow,
    allowed,
    not_allowed

}status_code;

typedef struct Stack_elem_int
{
    int data;
    struct Stack_elem_int* next;

}Stack_elem_int;

typedef struct
{
    Stack_elem_int* top;

}Stack_int;

void create_stack_int(Stack_int* stack)
{
    stack->top = NULL;
}

void push_int(Stack_int* stack, int data) 
{
    Stack_elem_int* new_elem = (Stack_elem_int*)malloc(sizeof(Stack_elem_int));
    if(!new_elem)
    {
        return;
    }

    new_elem->data = data;
    new_elem->next = stack->top;
    stack->top = new_elem;
}

int pop_int(Stack_int** stack)
{
    Stack_elem_int* out = (*stack)->top;
    int value = out->data;
    (*stack)->top = (*stack)->top->next;
    free(out);

    return value;
}

int peek_int(Stack_int* stack)
{
    if(stack->top == NULL)
    {
        return empty;
    }

    return stack->top->data;
}

void destroy_stack_int(Stack_int* stack)
{
    Stack_elem_int* current = stack->top;
    while(current != NULL)
    {
        Stack_elem_int* next = current->next;
        free(current);
        current = next;
    }
    stack->top = NULL;
}

int is_empty_int(Stack_int* stack)
{
    return (stack->top == NULL);
}
//////////char stack

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

int priotity(const char op)
{
    int flag = 0;
    if(op == '(' || op == ')')
    {
        flag = 0;
    }
    else if(op == '+' || op== '-')
    {
        flag = 1;
    }
    else if(op == '*' || op == '/' || op == '%')
    {
        flag = 2;
    }
    else if(op == '^')
    {
        flag = 3;
    }
    return flag;
}

int binary_pow(int base, int pow)
{
    if(pow == 0)
    {
        return 1;
    }
    if(pow == 1)
    {
        return base;
    }
    if(pow & 1) // nechet
    {
        return binary_pow(base, pow - 1) * base;
    }
    else
    {
        int number = binary_pow(base, pow >>= 1);
        return number * number;
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
            }
            if(!is_empty(stack) && peek(stack) == '(')//del all brackets
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


char* create_error_file(const char* filename)
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

status_code solve_expression(const char* postfix, int* result) 
{
    Stack_int* stack = (Stack_int*)malloc(sizeof(Stack_int));
    if(!stack)
    {
        return memory_error;
    }

    create_stack_int(stack);
    
    int length = strlen(postfix);
    int operand_1 = 0;
    int operand_2 = 0;

    for(int i = 0; i < length; i++)
    {
        if(isdigit(postfix[i]))
        {
            int number = 0;
            while(isdigit(postfix[i]))
            {
               number = number * 10.0 + (postfix[i] - '0');
                i++; 
            }
            push_int(stack, number);
        }
        else if(is_operator(postfix[i]))
        {
            operand_2 = pop_int(&stack);
            operand_1 = pop_int(&stack);

            switch(postfix[i])
            {
                case '+':
                    if(operand_1 > 0 && operand_2 > 0 && (INT_MAX - operand_2 < operand_1) || operand_1 < 0 && operand_2 < 0 && (INT_MIN - operand_2 > operand_1))
                    {
                        destroy_stack_int(stack);
                        return overflow;
                    }
                    else
                    {
                        push_int(stack, operand_1 + operand_2);
                    }
                    break;
                case '-':
                    push_int(stack, operand_1 - operand_2);
                    break;
                case '*':
                    if(INT_MAX / operand_1 <= operand_2)
                    {
                        destroy_stack_int(stack);
                        return overflow;
                    }
                    else
                    {
                        push_int(stack, operand_1 * operand_2);
                    }
                    break;
                case '/':
                    if(operand_2 == 0)
                    {   
                        destroy_stack_int(stack);
                        return division_by_zero;
                    }
                    else
                    {
                        push_int(stack, operand_1 / operand_2);
                    }
                    break;
                case '%':
                    if(operand_2 == 0)
                    {
                        destroy_stack_int(stack);
                        return division_by_zero;
                    }
                    else
                    {
                        push_int(stack, operand_1 % operand_2);
                    }
                    break;
                case '^':
                    if(operand_1 == 0 && operand_2 == 0)
                    {   
                        destroy_stack_int(stack);
                        return uncertainty;
                    }
                    else if(operand_2 < 0)
                    {
                        destroy_stack_int(stack);
                        return invalid_input;
                    }
                    else
                    {
                        push_int(stack, binary_pow(operand_1, operand_2));
                    }
                    break;
                default:
                    return invalid_input;

            }
                
        }
    }

    *result = stack->top->data;
    destroy_stack_int(stack);
    return success;
}

bool is_allowed_char(char c)
{
    char allowed[] = "0123456789+-*^%%/()";
    int len = strlen(allowed);
    for(int i = 0; i < len; i++)
    {
        if (c == allowed[i])
        {
            return true;
        }

    }
    return false;
}

status_code is_allowed(const char* string)
{
    int len = strlen(string);
    
    for(int i = 0; i < len; i++)
    {
        if(!is_allowed_char(string[i]))
        {
            return not_allowed;
        }
    }
    return allowed;
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

        status_code st_allowed = is_allowed(string);
        

        if(brackets_counter != 0)
        {
            fprintf(output_file,"infix: %s is not balanced with brackets in line: %d\n",string, line);
        }
        else if(st_allowed == not_allowed)
        {
            fprintf(output_file,"infix: %s detected the invalid character: %d\n",string, line);
        }
        else if(st_allowed == allowed)
        {
            printf("The infix: %s is balanced by brackets!\n", string);
            char polish[250];
            status_code status = get_reverse_poish(string, polish);
            if(status == success)
            {
                printf("polish expression: %s\n", polish);
            }
            else
            {
                fprintf(output_file,"infix: %s can't solve expression due to memory error: %d\n",string, line);
            }

            /////solve

            int result_of_solve = 0;

            status_code status_solve = solve_expression(polish, &result_of_solve);
            if(status_solve == success)
            {
                printf("result of solve: %d\n", result_of_solve);
            }
            else
            {
                if(status_solve == division_by_zero)
                {
                    fprintf(output_file,"infix: %s can't solve expression due to detected division by zero: %d\n",string, line);
                }
                else if(status_solve == uncertainty)
                {
                    fprintf(output_file,"infix: %s can't solve expression due to detected uncertainty: %d\n",string, line);
                }
                else if(status_solve == memory_error)
                {
                    fprintf(output_file,"infix: %s can't solve expression due to memory error: %d\n",string, line);
                }
                else if(status_solve == invalid_input)
                {
                    fprintf(output_file,"infix: %s can't solve expression due to invalid input: %d\n",string, line);
                }
                else
                {
                    fprintf(output_file,"infix: %s can't solve expression due to overflow: %d\n",string, line);
                }
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
        free(out);
    }

    
    return 0;
}
