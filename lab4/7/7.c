#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

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

}List;

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
List* create_list(int capasity)
{
    List* new_list = (List*)malloc(sizeof(List));
    if (!new_list)
    {
        return NULL;
    }
    new_list->elems = (MemoryCell**)malloc(sizeof(MemoryCell*) * capasity);
    if (!new_list->elems)
    {
        free(new_list);
        return NULL;
    }
    for (int index = 0; index < capasity; index++)
    {
        new_list->elems[index] = NULL;
    }
    new_list->capasity = capasity;
    new_list->size = 0;

    return new_list;
}
MemoryCell* create_memory_cell(char* name, int value)
{
    MemoryCell* new_memory_cell = (MemoryCell*)malloc(sizeof(MemoryCell));
    if (!new_memory_cell)
    {
        return NULL;
    }
    new_memory_cell->name = strdup(name);
    new_memory_cell->value = value;
    return new_memory_cell;
}
int comparator(const void* a, const void* b)
{
    const MemoryCell* elem_a = *((const MemoryCell**)a);
    const MemoryCell* elem_b = *((const MemoryCell**)b);

    return strcmp(elem_a->name, elem_b->name);
}

status_code insert(List* list, MemoryCell* elem)
{
    if (list->size + 1 >= list->capasity)
    {
        list->capasity *= 2;
        MemoryCell** tmp = (MemoryCell**)realloc(list->elems, sizeof(MemoryCell*) * list->capasity);
        if (!tmp)
        {
            list->capasity /= 2;
            return memory_error;
        }
        list->elems = tmp;
    }
    list->elems[list->size] = elem;
    (list->size)++;
    qsort(list->elems, list->size, sizeof(MemoryCell*), comparator);

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
int binary_search(List* list, char* key)
{
    int left = 0;            
    int right = list->size; 
    int mid = 0;              
    while (left <= right)
    {
        mid = (left + right) / 2;
        if (list->elems[mid] == NULL)
        {
            return -1;  
        } 
        MemoryCell* elem = list->elems[mid];
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
void print_all(List* list)
{
    printf("All variables:\n");
    for (int index = 0; index < list->size; index++)
    {
        printf("%s %d\n", list->elems[index]->name, list->elems[index]->value);
    }
}
void print_elem(List* list, int index)
{
    printf("The value of variable: %s - %d\n", list->elems[index]->name, list->elems[index]->value);
}

status_code process_expression(char* str, List* list)
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
    int index_1 = binary_search(list, first_part);
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
                    list->elems[index_1]->value = atoi(second_part);
                else
                {
                    MemoryCell *new_el = create_memory_cell(first_part, atoi(second_part));
                    if (new_el == NULL)
                    {
                        return memory_error;
                    }
                    if (insert(list, new_el) != success)
                    {
                        return memory_error;
                    }
                    return success;
                }
                return success;
            }
            else
            {
                int index_2 = binary_search(list, second_part);
                if (index_2 != -1)
                {
                    if (is_exist)
                    {
                        list->elems[index_1]->value = list->elems[index_2]->value;
                        return success;
                    }
                    else
                    {
                        MemoryCell *new_al = create_memory_cell(first_part, list->elems[index_2]->value);
                        if (insert(list, new_al) != success)
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
                int index_2 = binary_search(list, second_part);
                if (index_2 != -1)
                {
                    atoi_res_second_part = list->elems[index_2]->value;
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
                int index_3 = binary_search(list, third_part);
                if (index_3 != -1)
                {
                    atoi_res_third_part = list->elems[index_3]->value;
                }
                else
                    return invalid_value;
            }
            switch (operator)
            {
            case '+':
                atoi_res_second_part += atoi_res_third_part;
                break;
            case '-':
                atoi_res_second_part -= atoi_res_third_part;
                break;
            case '*':
                atoi_res_second_part *= atoi_res_third_part;
                break;
            case '/':
                atoi_res_second_part /= atoi_res_third_part;
                break;
            case '%':
                atoi_res_second_part %= atoi_res_third_part;
                break;
            default:
                break;
            }
            if (is_exist)
            {
                list->elems[index_1]->value = atoi_res_second_part;
                return success;
            }
            else
            {
                MemoryCell *new_el = create_memory_cell(first_part, atoi_res_second_part);
                if (new_el == NULL)
                {
                    return memory_error;
                }
                if (insert(list, new_el) != success)
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
            print_all(list);
            return success;
        }
        else
        {
            char* name = &(first_part[6]);
            int index = binary_search(list, name);
            if (index == -1)
            {
                return invalid_value;
            }
            print_elem(list, index);
            return success;
        }
    }
}
void destroy_list(List* list)
{
    for (int index = 0; index < list->capasity; index++)
    {
        if (list->elems[index] != NULL)
        {
            free(list->elems[index]->name);
        }
        free(list->elems[index]);
    }
    free(list->elems);
    free(list);
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

    List* list = create_list(10);
    char *expression;
    while (expression = read_expression_from_file(input_file))
    {   
        // printf("expression from file: %s\n", expression);
        if (process_expression(expression, list) != success)
        {
            printf("memory error or unknown value\n");
            destroy_list(list);
            free(expression);
            fclose(input_file);
            return 0;
        }
        free(expression);
    }
    
    destroy_list(list);
    fclose(input_file);
    return 0;
}
