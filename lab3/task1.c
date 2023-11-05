#include <stdio.h>
#include <stdlib.h>

enum status_code
{
    SUCCESS = 0,
    MEMORY_ERROR = 1,
    INVALID_INPUT = 2
};

void print_status_codes(int s_c)
{
    switch(s_c)
    {
        case MEMORY_ERROR:
            printf("failed to allocate memory\n");
            break;

        case INVALID_INPUT:
            printf("invalid input\n");
            break;

        default:
            break;
    }
}

int bin_exp(int r)//stepen'
{
    int res;
    res = 1 << r;
    return res;
}

int add(int a, int b)//slozhenie
{
    while (b != 0)
    {
        int carry = a & b;
        a = a ^ b;
        b = carry << 1;
    }
    return a;
}

int negative(int a) //otricatelnoe
{
    return add(~a, 1);
}

int substruction(int a, int b)//vychitanie
{
    return add(a, negative(b));
}

enum status_code convert_to_base(int number, int r, char** result) 
{
    int remainder = 0;
    int flag = 0;
    int base = bin_exp(r);

    if (r < 1 || r > 5) 
    {
        return INVALID_INPUT;
    }

    if (number < 0)
    {
        number = negative(number);
        flag = 1;
    }

    int count = 0;
    int new_number = number;

    while (new_number) 
    {
        count = add(count, 1);
        new_number >>= r; // new_number / (2^r)
    }

    if (flag) 
    {
        count = add(count, 1);
        *result = (char*)malloc(sizeof(char) * add(count,1));
        if (*result == NULL)
        {
            return MEMORY_ERROR;
        }
        (*result)[count] = '\0';
        (*result)[0] = '-';
    }
    else
    {
        *result = (char*)malloc(sizeof(char) * add(count, 1));
        if (*result == NULL)
        {
            return MEMORY_ERROR;
        }
        (*result)[count] = '\0';
    }

    while (number) 
    {
        remainder = number & substruction(base, 1); //ostatok ot deleniya
        count = substruction(count, 1);
        (*result)[count] = (remainder < 10) ? add(remainder, '0') : substruction(add(remainder, 'A'), 10);
        number >>= r;
    }

    return SUCCESS;
}

int main()
{
    char* result = NULL;
    int number = 0;
    int r = 0;

    printf("Enter the number: <decimal> \n");

    if (!(scanf("%d", &number)))
    {
        printf("invalid input\n");
        return INVALID_INPUT;
    }

    printf("Enter the degree: <1-5> \n");

    if (!(scanf("%d", &r))) 
    {
        printf("invalid input\n");
        return INVALID_INPUT;
    }

    enum status_code res = convert_to_base(number, r, &result);

    if (res == SUCCESS)
    {
        printf("number %d in %d base: \n", number, 1 << r);
        printf("%s\n", result);
    }
    else
    {
        print_status_codes(res);
    }

    free(result);

    return 0;
}
