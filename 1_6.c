#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

enum status_code 
{
    SUCCESS = 0,
    INVALID_INPUT = 1
};

char* del_zero(char* epsilon)
{
    char* number_without_zeros = epsilon;

    while (*number_without_zeros != '0')
    {
        number_without_zeros++;
        if(*number_without_zeros == '.')
        {
            number_without_zeros--;
        }
    }
    
    return number_without_zeros;
}

enum status_code check_epsilon(char* str)
{
    int dot_count = 0;
    int i = 0;
    int length = strlen(str);

    if(length < 3)
    {
        return INVALID_INPUT;
    }

    if(str[i] == '-' || str[i] == '+')
    {
        i++;
    }
    for(;i < length; i++)
    {
        if(str[i] == '.')
        {
            dot_count++;
        }
        else if (str[i] < '0' || str[i] > '9')
        {
            return INVALID_INPUT;
        }
    }
    if(dot_count != 1)
    {
        return INVALID_INPUT;
    }
   
    return SUCCESS;
    

}

enum status_code check_input(int argc, char* argv[])
{
    if (argc != 2)
    {
        return INVALID_INPUT;
    }

    if (check_epsilon(argv[1]) == INVALID_INPUT)
    {
        return INVALID_INPUT;
    }
    return SUCCESS;
}

double a_function(double x) 
{
    return log(1 + x) / x;
}

double b_function(double x)
{
    return exp(-pow(x, 2) / 2);
}

double c_function(double x) 
{
    if (x == 1 || x <= 0)
    {
        return 0.1;
    }
    return log(1/(1 - x));
}

double d_function(double x)
{
    return pow(x, x);
}

double right_rectangle(double a, double b, double n,double (*function)(double), double epsilon)
{
    double h = (b - a) / n; //step n-count of rectangles, a- start, b - end
    double sum = 0.0;
    double new_sum = 0.1 + epsilon;
    while (fabs(new_sum - sum) > epsilon) //acuurancy
    {
        sum = new_sum;
        new_sum = 0.0;
        for (int i = 1; i <= n; i++)
        {
            new_sum += h * (function(a + i * h));
        }
    }
    return new_sum;
}




int main(int argc, char* argv[]) //argv[1] = epsilon
{   

    if(check_input(argc, argv) != SUCCESS)
    {
        printf("invalid input\n");
    }
    else
    {   
        char* epsilon_with_zero = del_zero(argv[1]);//delete zeros exept 1 zero before '.'
        double epsilon = atof(epsilon_with_zero);
        printf("the result of integral a: %lf\n", right_rectangle(0, 1, 100, a_function, epsilon));
        printf(" the result of integral b: %lf\n",right_rectangle(0, 1, 100, b_function, epsilon));
        printf("the result of integral integral c: %lf\n",right_rectangle(0, 1, 100, c_function, epsilon));
        printf("the result of integral d: %lf\n", right_rectangle(0, 1, 100, d_function, epsilon));

    }



    return 0;
}
