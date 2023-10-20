
#include <stdio.h>
#include <stdarg.h>
#include <math.h>

double geometric_mean(int count, ...)
{
    double value = 1.0;
    va_list ptr;
    va_start(ptr, count);

    for (int i = 0; i < count; i++)
    {
        value *= va_arg(ptr, double);

    }
    va_end(ptr);
    double result = pow(value, 1.0/count);
    return result;

}

double binary_pow(double number, int degree)
{
    if (degree == 0)
    {
        return 1.0;
    }
    if (degree == 1)
    {
        return number;
    }
    if (degree == - 1)
    {
        return 1 / number;
    }

    if ( degree & 1 == 1)
    {
        return binary_pow(number, degree - 1) * number;
    }
    else
    {
        double res = binary_pow(number, degree / 2);
        return res * res;
    }
}
int main()
{
    double geometric_mean_res = geometric_mean(3, 2.0, 2.0, 2.0);
    printf("The result of geometric mean: %f\n", geometric_mean_res);

    double binary_pow_result = binary_pow(2.0, -2);
    printf("The result of binary power: %f\n", binary_pow_result);
    
return 0;
}
