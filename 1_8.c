#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#define SIZE 200

char *sc = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

enum STATUS_CODE
{
    success = 0,
    error = 1
};

char *del_zero(char *number) {
    char *number_without_zeros = number;

    while (*number_without_zeros && *number_without_zeros == '0') {
        number_without_zeros++;
    }

    if (!*number_without_zeros) {
        number_without_zeros--;
    }

    return number_without_zeros;
}

int convert_to_decimal(char* str, int base)
{
    if (!str)
    {
        return error;
    }

    int sign = 1;
    char* ptr = NULL;
    int result = 0;

    if(*str == '-')
    {
        sign = -1;
        ptr = str + 1;
    }

    else
    {
        ptr = str;
    }

    while (*ptr)
    {
        result = result * base + (isdigit(*ptr) ? *ptr - '0' : *ptr - 'A' + 10);
        ++ptr;
    }

    return result * sign;
}

int read_my_sym(char s)
{
    if ((s > 32 && s <= 47 && s != '-') || (s >= 58 && s <= 64) || (s >= 91 && s <= 96) || (s >= 123))
        return error;
    else
        return success;
}

int find_min_base(char *number, int len)
{
    int r = 0;
    int i = 0;
    if(number[0] == '-') ++i;
    for (;i < len; ++i)
    {
        if(number[i] > sc[r])
        {
            if(number[i] >= '0' && number[i] <= '9')
            {
                r = number[i] - '0';
            }

            if(number[i] >= 'A' && number[i] <= 'Z')
            {
                r = number[i] - 'A' + 10;
            }
        }
        if (r == 0)
        {
            return r + 2;
        }
    }
    return r + 1;
}

int valid_num(char* number)
{
    int minus_count = 0;
    int i = 0;
    if (strlen(number) == 1 && number[0] == '-')
    {
        return error;
    }
    for(i = 0; i < strlen(number); ++i)
    {
        if(number[i] == '-')
        {
            ++minus_count;
            if (i != 0)
            {
                return error;
            }
            ++i;
        }
        if(isalpha(number[i]) || read_my_sym(number[i]))
        {
            return error;
        }
    }
    if(minus_count > 1)
    {
        return error;
    }
    return success;
}

int read_my_file(FILE *input_file, FILE *output_file)
{
    char symbol;
    char num[SIZE];
    int i = 0;
    int base = 0;
    int decimal = 0;

    while((symbol = fgetc(input_file)) != EOF)
    {
        while (symbol == ' ' || symbol == '\t' || symbol == '\n')
        {
            symbol = fgetc(input_file);
        }
        while (isdigit(symbol) || isalpha(symbol) || read_my_sym(symbol) || symbol == '-')
        {
            num[i] = symbol;
            ++i;
            symbol = fgetc(input_file);
        }

        num[i] = '\0';
        char *num_without_zeros = del_zero(num);

        if(valid_num(num_without_zeros) == success)
        {
            printf("%s\n", num_without_zeros);
            base = find_min_base(num_without_zeros, i);
            decimal = convert_to_decimal(num_without_zeros, base);
            fprintf(output_file, "%s %d %d\n", num_without_zeros, base, decimal);
        }

        i = 0;
    }

    return error;

}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: input_file output_file\n");
        return 1;
    }

    FILE *input_file = fopen(argv[1], "r");
    if (!input_file)
    {
        printf("Cannot open input file\n");
        return 1;
    }

    FILE *output_file = fopen(argv[2], "w");
    if (!output_file)
    {
        printf("Cannot open output file\n");
        return 1;
    }

    read_my_file(input_file, output_file);

    fclose(input_file);
    fclose(output_file);
    return 0;
}
