#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdlib.h>

enum status_code
{
    SUCCESS = 0,
    INVALID_INPUT = 1,
    MEMORY_ERROR = 2,
    YES = 3,
    NO = 4
};

void print_status(enum status_code s_c)
{
    switch(s_c)
    {
        case INVALID_INPUT:
            printf("invalid input\n");
            break;

        case MEMORY_ERROR:
            printf("failed to allocate memory\n");
            break;


        default:
            break;
    }
}

int greatest_common_divisor(int a, int b)
{
    if (a == 0)
    {
        return b;
    }
    else if (a < 0 || b < 0)
    {
        return greatest_common_divisor(abs(a), abs(b));
    }
    else
    {
        return greatest_common_divisor(b % a, a);
    }
}

bool is_prime(int a)
{
    if (a <= 1) 
    {
        return false;
    }
    for (int i = 2; i * i <= a; i++) 
    {
        if (a % i == 0) 
        {
            return false;
        }
    }
    return true;
}

enum status_code prime_in_base(int base, int numerator, int denominator)
{
    for (int i = 2; i <= denominator; i++)
    {
        if(denominator % i == 0)
        {
            if(is_prime(i) && base % i != 0)
            {
                return NO;
            }
        }
    }
    return YES;
}

enum status_code finite_representation(int** res, unsigned int base, int count, ...)
{
    if (base < 2 || base > 36 || count < 1)
    {
        return INVALID_INPUT;
    }

    *res = (int*)malloc(sizeof(int) * count);
    if (*res == NULL)
    {
        return MEMORY_ERROR;
    }
    
    va_list ptr;
    va_start(ptr, count);

    for (int i = 0; i < count; i++)
    {
        double value = va_arg(ptr, double);

        int numerator, denominator = 1;

        while(value > (int)value)
        {
            value *= 10;
            denominator *= 10;
        }

        numerator = (int)value; 

        int nod = greatest_common_divisor(numerator,denominator); 
        
        numerator /= nod;
        denominator /= nod;

        enum status_code res_prime_in_base = prime_in_base(base, numerator, denominator);
        
        if (res_prime_in_base == YES)
        {
            (*res)[i] = 1; 
        }
        else
        {
            (*res)[i] = 0; 
        }

    }

    va_end(ptr);
    return SUCCESS;


}

int main()

{
    unsigned int base = 0;
    printf("Enter base ");

    if (scanf("%d", &base) != 1)
    {
        print_status(INVALID_INPUT);
        return INVALID_INPUT;

    }

    int *res;
    int count = 5;
    double num1 = -0.375, num2 = 0.3, num3 = -0.2, num4 = 0.25, num5 = 0.1;

    enum status_code result_status = finite_representation(&res, base, count, num1, num2, num3, num4, num5);

    if (result_status == SUCCESS)
    {
        for (int i = 0; i < count; i++)
        {
            printf("%d ", res[i]);
        }
    }
    else
    {
        print_status(result_status);
        return result_status;
    }
    free(res);

    return 0;
}
