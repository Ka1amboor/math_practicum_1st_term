#include <stdio.h>
#include <math.h>

enum STATUS_CODE
{
    SUCCESS = 0,
    NO_ROOT = 1,
    INVALID_EPSILON = 2
};

double function(double x)
{
    return x * x - 4.0;
}

enum STATUS_CODE root(double a, double b, double epsilon, double (*function_ptr)(double), double *result)
{   

    if (epsilon <= 0)
    {
        return INVALID_EPSILON;
    }
    if (function_ptr(a) * function_ptr(b) > 0)
    {
        return NO_ROOT;
    }
    while (fabs(b - a) > epsilon)
    {
        *result = (a + b) / 2.0;
        if (function_ptr(b) * function_ptr(*result) < 0)
        {
            a = *result;
        }
        else
        {
            b = *result;
        }
        
    }

    return SUCCESS;
}

int main()
{
    double result = 0;
    double a = 0.0;
    double b = 3.0;
    double epsilon = 0.000001;

    enum STATUS_CODE answer = root(a, b, epsilon, function, &result);
    if (answer == SUCCESS)
    {
        printf("The root of the equation: %f\n", result);
    }
    return 0;
}
