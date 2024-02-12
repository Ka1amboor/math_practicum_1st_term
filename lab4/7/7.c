#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>

typedef struct
{
    int value;
    char* name;

}MemoryCell;

typedef struct
{
    MemoryCell** elems;
    int size;
    int capasity;

}Array;

typedef enum
{
    success,
    memory_error,
    error_with_opening_file,
    invalid_input,
    invalid_value

}status_code;

int is_int(char* string)
{
    while (*string)
    {
        if (!isdigit(*string)) return 0;
        string++;
    }
    return 1;
}
Array* create_Array(int capasity)
{
    Array* new_Array = (Array*)malloc(sizeof(Array));
    if (!new_Array)
    {
        return NULL;
    }
    new_Array->elems = (MemoryCell**)malloc(sizeof(MemoryCell*) * capasity);
    if (!new_Array->elems)
    {
        free(new_Array);
        return NULL;
    }
    for (int index = 0; index < capasity; index++)
    {
        new_Array->elems[index] = NULL;
    }
    new_Array->capasity = capasity;
    new_Array->size = 0;

    return new_Array;
}
MemoryCell* create_memory_cell(char* name, int value)
{
    MemoryCell* new_memory_cell = (MemoryCell*)malloc(sizeof(MemoryCell));
    if (!new_memory_cell)
    {
        return NULL;
    }
    new_memory_cell->name = strdup(name);
    if(new_memory_cell->name == NULL)
    {
        free(new_memory_cell);
        return NULL;
    }
    new_memory_cell->value = value;
    return new_memory_cell;
}
int comparator(const void* a, const void* b)
{
    const MemoryCell* elem_a = *((const MemoryCell**)a);
    const MemoryCell* elem_b = *((const MemoryCell**)b);

    return strcmp(elem_a->name, elem_b->name);
}

status_code insert(Array* Array, MemoryCell* elem)
{
    if (Array->size + 1 >= Array->capasity)
    {
        Array->capasity *= 2;
        MemoryCell** tmp = (MemoryCell**)realloc(Array->elems, sizeof(MemoryCell*) * Array->capasity);
        if (!tmp)
        {
            Array->capasity /= 2;
            return memory_error;
        }
        Array->elems = tmp;
    }
    Array->elems[Array->size] = elem;
    (Array->size)++;
    qsort(Array->elems, Array->size, sizeof(MemoryCell*), comparator);

    return success;
}
char* read_expression_from_file(FILE *file)
{
    int size = 2;
    char sym;
    int index = 0;

    char* expression = (char*)malloc(sizeof(char) * size);
    if (!expression)
    {
        return NULL;
    }
    sym = fgetc(file);
    while (sym != EOF && sym != ';')
    {
        if (index >= size)
        {
            size *= 2;
            char* tmp = (char*)realloc(expression, sizeof(char) * size);
            if (!tmp)
            {
                free(expression);
                return NULL;
            }
            expression = tmp;
        }
        expression[index] = sym;
        index++;
        sym = fgetc(file);
    }
    if (index == 0)
    {
        free(expression);
        return NULL;
    }
    expression[index] = ';';
    expression[index + 1] = '\0';
    return expression;
}
int binary_search(Array* Array, char* key)
{
    int left = 0;
    int right = Array->size;
    int mid = 0;
    while (left <= right)
    {
        mid = (left + right) / 2;
        if (Array->elems[mid] == NULL)
        {
            return -1;
        }
        MemoryCell* elem = Array->elems[mid];
        int res_strcmp = strcmp(key, elem->name);
        if (res_strcmp == 0)
        {
            return mid;
        }
        else if (res_strcmp > 0)
        {
            left = mid + 1; //for less steps
        }
        else
        {
            right = mid - 1;
        }
    }
    return -1;
}
int is_operator(char sym)
{
    return (sym == '+' || sym == '-' || sym == '*' || sym == '/' || sym == '%') ? 1 : 0;
}
void print_all(Array* Array)
{
    printf("All variables:\n");
    for (int index = 0; index < Array->size; index++)
    {
        printf("%s %d\n", Array->elems[index]->name, Array->elems[index]->value);
    }
}
void print_elem(Array* Array, int index)
{
    printf("The value of variable: %s - %d\n", Array->elems[index]->name, Array->elems[index]->value);
}

int is_overflow(int a, int b)
{
    if (a > 0)
    {
        if (b > 0)
        {
            if (a > (INT_MAX / b))
            {
                return 1;
            }
        }
        else
        {
            if (b < (INT_MIN / a))
            {
                return 1;
            }
        }
    }
    else
    {
        if (b > 0)
        {
            if (a < (INT_MIN / b))
            {
                return 1;
            }
        }
        else
        {
            if ((a != 0) && (b < (INT_MAX / a)))
            {
                return 1;
            }
        }
    }
    return 0;
}

status_code process_expression(char* str, Array* Array)
{
    char first_part[100];
    char second_part[100];
    char third_part[100];
    int index = 0;
    int fp_index = 0;
    while (!isalnum(str[index]))
        index++;
    //first part
    while (str[index] != ';' && str[index] != '=')
    {
        first_part[fp_index] = str[index];
        index++;
        fp_index++;
    }
    first_part[fp_index] = '\0';
    int is_exist = 0;
    int index_1 = binary_search(Array, first_part);
    if (index_1 != -1)
        is_exist = 1;
    if (str[index] == '=')
    {
        index++;
        int index_2 = 0;
        //second part
        while (str[index] != ';' && !is_operator(str[index]))
        {
            second_part[index_2] = str[index];
            index++;
            index_2++;
        }
        second_part[index_2] = '\0';
        if (str[index] == ';')
        {
            if (is_int(second_part))
            {
                if (is_exist)
                    Array->elems[index_1]->value = atoi(second_part);
                else
                {
                    MemoryCell *new_el = create_memory_cell(first_part, atoi(second_part));
                    if (new_el == NULL)
                    {
                        return memory_error;
                    }
                    if (insert(Array, new_el) != success)
                    {
                        return memory_error;
                    }
                    return success;
                }
                return success;
            }
            else
            {
                int index_2 = binary_search(Array, second_part);
                if (index_2 != -1)
                {
                    if (is_exist)
                    {
                        Array->elems[index_1]->value = Array->elems[index_2]->value;
                        return success;
                    }
                    else
                    {
                        MemoryCell *new_al = create_memory_cell(first_part, Array->elems[index_2]->value);
                        if (insert(Array, new_al) != success)
                        {
                            return memory_error;
                        }
                        return success;
                    }
                }
                else
                    return invalid_value;
            }
        }
        if (is_operator(str[index]))
        {
            char operator = str[index];
            index++;
            int index_3 = 0;
            while (str[index] != ';' && str[index] != '\0' && str[index] != EOF)
            {
                third_part[index_3] = str[index];
                index++;
                index_3++;
            }
            third_part[index_3] = '\0';
            int atoi_res_second_part;
            if (is_int(second_part))
            {
                atoi_res_second_part = atoi(second_part);
            }
            else
            {
                int index_2 = binary_search(Array, second_part);
                if (index_2 != -1)
                {
                    atoi_res_second_part = Array->elems[index_2]->value;
                }
                else
                    return invalid_value;
            }
            int atoi_res_third_part;
            if (is_int(third_part))
            {
                atoi_res_third_part = atoi(third_part);
            }
            else
            {
                int index_3 = binary_search(Array, third_part);
                if (index_3 != -1)
                {
                    atoi_res_third_part = Array->elems[index_3]->value;
                }
                else
                    return invalid_value;
            }
            switch (operator)
            {
                case '+':
                    if (((atoi_res_third_part > 0) && (atoi_res_second_part > (INT_MAX - atoi_res_third_part))) ||
                        ((atoi_res_third_part < 0) && (atoi_res_second_part < (INT_MAX - atoi_res_third_part))))
                    {
                        return invalid_value;
                    }
                    atoi_res_second_part += atoi_res_third_part;
                    break;
                case '-':
                    if ((atoi_res_third_part > 0 && atoi_res_second_part < INT_MIN + atoi_res_third_part) ||
                        (atoi_res_third_part < 0 && atoi_res_second_part > INT_MAX + atoi_res_third_part))
                    {
                        return invalid_value;
                    }
                    atoi_res_second_part -= atoi_res_third_part;
                    break;
                case '*':
                    if (is_overflow(atoi_res_second_part, atoi_res_third_part))
                    {
                        return invalid_value;
                    }
                    atoi_res_second_part *= atoi_res_third_part;
                    break;
                case '/':
                    if ((atoi_res_third_part == 0) || ((atoi_res_second_part == INT_MIN) && (atoi_res_third_part == -1)))
                    {
                        return invalid_value;
                    }
                    atoi_res_second_part /= atoi_res_third_part;
                    break;
                case '%':
                    if ((atoi_res_third_part == 0) || ((atoi_res_second_part == INT_MIN) && (atoi_res_third_part == -1)))
                    {
                        return invalid_value;
                    }
                    atoi_res_second_part %= atoi_res_third_part;
                    break;
                default:
                    break;
            }
            if (is_exist)
            {
                Array->elems[index_1]->value = atoi_res_second_part;
                return success;
            }
            else
            {
                MemoryCell *new_el = create_memory_cell(first_part, atoi_res_second_part);
                if (new_el == NULL)
                {
                    return memory_error;
                }
                if (insert(Array, new_el) != success)
                {
                    return memory_error;
                }
                return success;
            }
        }
    }
    if (str[index] == ';')
    {
        if (strcmp(first_part, "print") == 0) //len = 5
        {
            print_all(Array);
            return success;
        }
        else
        {
            char* name = &(first_part[6]);
            int index = binary_search(Array, name);
            if (index == -1)
            {
                return invalid_value;
            }
            print_elem(Array, index);
            return success;
        }
    }
    else
    {
        return invalid_input;
    }
    //TODO: return invalid_input;
}
void destroy_Array(Array* Array)
{
    for (int index = 0; index < Array->capasity; index++)
    {
        if (Array->elems[index] != NULL)
        {
            free(Array->elems[index]->name);
        }
        free(Array->elems[index]);
    }
    free(Array->elems);
    free(Array);
}

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        printf("invalid input!\n Usage: <input_file.txt>\n");
        return invalid_input;
    }

    FILE* input_file = fopen(argv[1], "r");
    if(!input_file)
    {
        printf("error with opening file\n");
        return error_with_opening_file;
    }

    Array* Array = create_Array(10);
    char *expression;
    while (expression = read_expression_from_file(input_file))
    {
        // printf("expression from file: %s\n", expression);
        if (process_expression(expression, Array) != success)
        {
            printf("memory error or unknown value\n");
            destroy_Array(Array);
            free(expression);
            fclose(input_file);
            return 0;
        }
        free(expression);
    }

    destroy_Array(Array);
    fclose(input_file);
    return 0;
}
