#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>

typedef enum
{
    SUCCESS = 0,
    WRONG_BASE = 1,
    MEMORY_ERROR = 2,
    INVALID_VALUE = 3,
    ERROR = 4

}status_code;

int my_strlen(char* str)
{
    char* end_ptr = str;
    char* start_ptr = str;

    while(*end_ptr)
    {
        end_ptr++;
    }

    return end_ptr - start_ptr;
}

int convert_to_int(char c) 
{
    if (isdigit(c)) 
    {
        return (int)c - '0';
    }
    if (isalpha(c)) 
    {
        c = toupper(c);
        return (int)c - 'A' + 10;
    }

    return -1;
}

void sdvig(char* str) 
{
    int ind = 0;
    while (str[ind] != '\0') 
    {
        str[ind] = str[ind+1];
        ind++;
    }
}

status_code sum_two_numbers(char* num_1, char* num_2, int base, char** res)
{
    int len_num_1 = my_strlen(num_1);
    int len_num_2 = my_strlen(num_2);

    if(len_num_1 == 0|| len_num_2 == 0)
    {
        return INVALID_VALUE;
    }

    int max_len = (len_num_1 > len_num_2) ? len_num_1 : len_num_2;
    *res = (char*)malloc((max_len + 2) * sizeof(char));

    if(*res == NULL)
    {
        return MEMORY_ERROR;
    }

    for(int i = 0; i <= max_len + 1; i++)
    {
        (*res)[i] = '0';
    }

    int carry = 0;
    int sum = 0;

    for(int i = 0; i < max_len; i++)
    {
        int a = i < len_num_1 ? convert_to_int(num_1[len_num_1-i-1]) : 0;
        int b = i < len_num_2 ? convert_to_int(num_2[len_num_2-i-1]) : 0;

        if (a >= base || b >= base || a == -1 || b == -1) 
        {
            free(*res);
            return INVALID_VALUE;
        }
        sum = carry + a + b;
        (*res)[max_len-i] = (sum % base) + ((sum % base < 10) ? '0' : 'A' - 10);
        carry = sum / base;
    }
    if (carry > 0) 
    {
        (*res)[0] = carry + ((carry < 10) ? '0' : 'A' - 10);
        (*res)[max_len+1] = '\0';
    } else 
    {
        sdvig(*res);
        (*res)[max_len] = '\0';
    }

    return SUCCESS;
}

void remove_zeroes(char* str) {
    int ind = 0;
    while (str[ind] != '\0' && str[ind] == '0') {
        ind++;
    }
    if (str[ind] == '\0') {
        str[1] = '\0';
        return;
    }
    int ind1 = ind;
    while (str[ind1] != '\0') {
        str[ind1 - ind] = str[ind1];
        ind1++;
    }
    str[ind1 - ind] = '\0';
}

int sum(char** result, int base, int count, ...) 
{
    if (base < 2 || base > 36 || count < 2) 
    {
        return WRONG_BASE;
    }

    va_list args;
    va_start(args, count);

    *result = (char*)malloc(2 * sizeof(char));
    if (*result == NULL)
    {
        return MEMORY_ERROR;
    }
    (*result)[0] = '0';
    (*result)[1] = '\0';

    for (int i = 0; i < count; i++) 
    {
        char* arg = va_arg(args, char*);
        if (my_strlen(arg) == 0) 
        {
            continue;
        }
        char* tmp = (char*)malloc(sizeof(char) * (my_strlen(*result) + 1));
        if (tmp == NULL)
        {
            free(*result);
            return MEMORY_ERROR;
        }

        strcpy(tmp, *result);
        free(*result);
        status_code st = sum_two_numbers(tmp, arg, base, result);
        if (st != SUCCESS) 
        {
            free(tmp);
            free(*result);
            return MEMORY_ERROR;
        }
        free(tmp);
    }
    va_end(args);
    remove_zeroes(*result);
    return SUCCESS;
}


int main() 
{
    char* result = NULL;
    char* value_1 = "A";
    char* value_2 = "A";
    char* value_3 = "A";
    int base = 16;
    int count = 3;

    status_code st = sum(&result, base, count, value_1, value_2, value_3);

    switch(st)
    {
    case WRONG_BASE:
        printf("wrong base\n");
        free(result);
        break;

    case MEMORY_ERROR:
        printf("failed to allocate memory\n");
        free(result);
        break;

    case INVALID_VALUE:
        printf("invalid value!\n");
        free(result);
        break;

    case SUCCESS:
        printf("%s\n", result);
        free(result);
        break;

    default:
        break;
    }

    return 0;
}
