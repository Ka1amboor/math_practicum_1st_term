#include <stdio.h>
#include <malloc.h>
#include <stdarg.h>
#include <math.h>
#include <stdlib.h>

typedef enum 
{
    SUCCESS = 0,
    INVALID_VALUE = 1,
    MEMORY_ERROR = 2

} status_code;

status_code polynom_decomposition (double epsilon, double a, double** res, int degree, ...) 
{
    double coefficient;
    int derivative_coef = 1, deg;
    long long int factorial = 1;

    double* new_coefficients = (double*)calloc((degree + 1), sizeof(double));
    if (new_coefficients == NULL) 
    {
        return MEMORY_ERROR;
    }

    va_list(ptr);
    va_start(ptr, degree);

    deg = 0;
    for (int n = 0; n <= degree; n++) 
    {
        coefficient = va_arg(ptr, double);

        int k = deg;
        while (k >= 0) 
        {
            if (fabs(coefficient) < epsilon) 
            {
                break;
            } 
            else 
            {
                new_coefficients[deg - k] += coefficient * pow(a, k) * derivative_coef;
            }

            if (k > 1) 
            {
                derivative_coef *= k;
            }
            k--;
        }
        derivative_coef = 1;
        deg++;
    }

    va_end(ptr);

    for (int i = 2; i <= degree; i++)
    {
        factorial *= i;
        new_coefficients[i] /= 1.0 * factorial;
    }

    *res = new_coefficients;
    return SUCCESS;
}

int main () 
{
    double *new_coefficients = NULL;
    double epsilon = 0.000001;
    int degree = 2;
    double a = 1.0;

    status_code sc = polynom_decomposition(epsilon, a, &new_coefficients, degree, 3.0, 4.0, 2.0);
    switch(sc)
    {
        case SUCCESS:
            for(int i = 0; i <= degree; i++)
            {
                printf("g%d: %f", i, new_coefficients[i]);
                printf("\n");
            }
            break;

        case MEMORY_ERROR:
            printf("failed to allocate memory\n");
            break;

    }

    free(new_coefficients);
    return 0;
}
