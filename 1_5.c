#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#define SIZE 33

bool check_my_base(int base)
{
    char str[SIZE];
    sprintf(str,"%d", base);
    for (int i = 0; i < strlen(str); i++ )
    {
        if (!isdigit(str[i]))
        {
            return false;
        }
    }
    return true;
}

bool check_my_string(char* number)
{
    if (!number)
    {
        return false;
    }
    int sign = 1;
    char* ptr = NULL;
    if (*number == '-')
    {
        sign = -1;
        ptr = number + 1;
    }
    else
    {
        ptr = number;
    }
    while(*++ptr)
    {
        if (!isalnum(*ptr))
        {
            return false;
        }
    }
    return true;
}

int min_base(char* number, int base)
{
    if (!number)
    {
        return 0;
    }
    int max_digit = 0;
    char* ptr = number - 1;
    while(*++ptr)
    {
        if(isdigit(*ptr))
        {
            if((*ptr - '0') > max_digit)
            {
                max_digit = *ptr - '0';
            }
        }
        else
        {
            if((*ptr - 'A' + 10) > max_digit)
            {
                max_digit = (*ptr - 'A' + 10);
            }
        }
    }
    return max_digit;
}

int convert_to_decimal(char* number, int base) 
{
    if (!number)
    {
        return 0;
    }
    int sign = 1;
    char *ptr = NULL;
    int result = 0;

    if (*number == '-')
    {
        sign = -1;
        ptr = number + 1;
    }

    else
    {
        ptr = number;
    }

    while (*ptr)
    {
        result = result * base + (isdigit(*ptr) ? *ptr  - '0' : *ptr - 'A' + 10);
        ptr++;

    }

    return result * sign;
    
}

char* convert_to_base(int number, int base) 
{
    int sign = 1;
    if (number < 0)
    {
        sign = -1;
        number = sign * number;
    }
    int r;
    static char buf[SIZE];
    char* ptr = buf + SIZE - 1;
    *ptr = '\0';
    while(number > 0)
    {
        r = number % base;
        *--ptr = (r > 9)? r - 10 + 'A' : r + '0';
        number /= base;
    }
    if (sign == -1)
    {
        *--ptr = '-';
    }
    return ptr;
}

int main() 
{
    int base;
    printf("Please, enter the base of the number system (2-36): ");
    scanf("%d", &base);
    if (base < 2 || base > 36) 
    {
        printf("ERROR! Invalid base!\n");
        return 1;
    }
    if(!(check_my_base(base)))
    {
        printf("ERROR DETECTED! Please use digit for base!");
        return 1;
    }
    char number[100];
    int max_abs = 0;
    while (1) 
    {
        printf("Enter a number in base %d (or enter 'Stop' to finish): ", base);
        scanf("%s", number);
        if (!(check_my_string(number)))
        {
            printf("ERROR DETECTED! There are something in the number besides letters and numbers!");
            return 1;
        }
        if ((strcmp(number, "Stop") == 0) || (strcmp(number, "stop") == 0))
        {
            break;
        }
        int min_base_number = min_base(number, base);
        if (min_base_number >= base)
        {
            printf("ERROR DETECTED! Invalid input: digits or letters are greater than the base!\n");
            return 1;
        }
        int decimal = convert_to_decimal(number, base);
        if(decimal == 0)
        {
            printf("ERROR DETECTED! Invalid input: zero is not allowed!\n");
            return 1;
        }
        int abs_decimal = abs(decimal);
        if (abs_decimal > abs(max_abs)) 
        {
            max_abs = decimal;

        }
    
    }
    printf("Max value: %d\n", max_abs);
    printf("Max value in base 9: %s\n", convert_to_base(max_abs, 9));
    printf("Max value in base 18: %s\n", convert_to_base(max_abs, 18));
    printf("Max value in base 27: %s\n", convert_to_base(max_abs, 27));
    printf("Max value in base 36: %s\n", convert_to_base(max_abs, 36));
    return 0;
}