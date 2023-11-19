#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <stdbool.h>

enum status_code
{
    SUCCESS = 0,
    INVALID_INPUT = 1
};

char* del_zeros(char* epsilon_with_zeros)
{
    char* num_without_zeros = epsilon_with_zeros;

    while(*num_without_zeros != '0')
    {
        num_without_zeros++;

        if(*num_without_zeros =='.')
        {
            num_without_zeros--;
        }
    }

    return num_without_zeros;
}

enum status_code check_epsilon(char* epsilon)
{
    int i = 0;
    int dot_count = 0;
    int len = strlen(epsilon);

    if(len < 3)
    {
        return INVALID_INPUT;
    }

    if(epsilon[i] == '-' || epsilon[i] == '+')
    {
        i++;
    }

    for(; i < len; i++)
    {
        if (epsilon[i] == '.')
        {
            dot_count++;
        }
        else if(epsilon[i] < '0' || epsilon[i] > '9')
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

    if(argc != 2)
    {
        return INVALID_INPUT;
    }

    if (check_epsilon(argv[1]) == INVALID_INPUT)
    {
        return INVALID_INPUT;
    }

    return SUCCESS;


}

double e_lim(double epsilon)
{
    double prev = 1;
    double current = prev;
    int n = 0;
    do 
    {
        n++;
        prev = current;
        current = pow((1 + 1.0/n), n);

    } while (fabs(current - prev) > epsilon);

    return current;
}

double pi_lim(double epsilon)
{
    int n = 1;
    double prev = 4; //because if n = 1, => first elem = 4
    double current = prev;
    do
    {
        prev = current;
        current = current * 4 * (n + 1) * n / ((2 * n + 1) * ( 2 * n + 1)); //for get (n + 1) elem
        n++;

    } while (fabs(prev - current) > epsilon);

    return current;

}

double ln_2_lim(double epsilon)
{
    int n = 1;
    double prev = 1;
    double current = prev;
    do
    {
        n++;
        prev = current;
        current = n * (pow(2, 1.0/(double)n) - 1);

    }while (fabs(current - prev) > epsilon);

    return current;
}

double sqrt_2_lim(double epsilon)
{
    double x0 = -0.5;
    double x1 = x0 - x0 * x0 / 2 + 1;

    do
    {
        x0 = x1;
        x1 = x0 - x0 * x0 / 2 + 1;

    } while (fabs(x1 - x0) > epsilon);

    return x1;
}

double factorial(double n)
{
    double res = 1;

    if ( n < 2)
    {
        return 1.0;
    }

    for (double  i = 1; i <= n; i++)
    {
        res *= i;
    }

    return res;
}


double gamma_lim(double epsilon)
{
    int m = 1;
    double prev = 0;
    double current = 0;

    do 
    {
        m++;
        prev = current;
        current = 0;
        double pl = 0;
        double combinations = m;
        double lg = 0;

        for (int k = 2; k <= m; k++) 
        {
            combinations *= (m-k+1.0) / k;
            lg += log(k);

            pl = combinations * lg / k;
            if (k & 1 != 0) //nechet
            {
                pl *= -1;
            }
            current += pl;
        }
    } while (fabs(prev - current) > epsilon);

    return current;
}

double e_row(double epsilon)
{
    double current = 2;
    double sum = current;
    double fact = 1;
    int n  = 1;

    do
    {
        n++;
        current = sum;
        fact *= n;
        sum += 1/fact;

    }while(fabs(current - sum) > epsilon);

    return sum;
}

double pi_row(double epsilon)
{
    double current;
    double sum = 0.0;
    double n = 1.0;

    while (fabs(current) >= epsilon)
    {
        current = pow(-1, n - 1)/(2 * n - 1);
        sum += current;
        n++;
    }
    return 4 * sum;
}

double ln_2_row(double epsilon)
{
    double current = 1.0;
    double sum = 0.0;
    double n = 1;

    while (fabs(current) >= epsilon)
    {
        current = pow(-1, n - 1) / n;
        sum += current;
        n++;
    }
    return sum;
}

double sqrt_2_row(double epsilon)
{
    double res_1 = 0.0;
    double r = pow(2.0, 0.25);
    double res_3 = 1;

    while (fabs(res_3 - res_1) >= epsilon)
    {
        res_1 = res_3;
        res_3 *= r;
        r = pow(r, 0.5);
    }
    return res_3;
}

double gamma_row(double epsilon)
{
    double old_sum = 0.0;
    double new_sum = 1.0;
    double pi = 3.14159265358979323846;
    int k = 2;
    do
    {
        old_sum = new_sum;
        double term = 1.0 / (pow(floor(sqrt(k)), 2)) - 1.0 / k;
        new_sum += term;
        if (fabs(term) < epsilon)
            break;
        k++;

    } while (fabs(new_sum - old_sum) > epsilon);

    return -pow(pi, 2) / 6 + new_sum;
}

double e_equation(double epsilon)
{
    return exp(1.0);
}

double pi_equation(double epsilon)
{
    return acos(-1.0);
}

double ln_2_equation(double epsilon)
{
    return log(2.0);
}

double sqrt_2_equation(double epsilon)
{
    return sqrt(2.0);
}


bool is_prime(int num)
{
    if (num > 1) 
    {
        for (int i = 2; i < sqrt(num); i++)
            if (num % i == 0)
                return false;
        return true;
    }
    else
        return false;
}

double gamma(double t)
{
    double mult = 1;

    for (int i = 2; i <= t; i++)
    {
        if (is_prime(i))
        {
            mult *= (i - 1.0) / i;
        }
    }
    return mult;
}

double gamma_equation(double epsilon)
{
    int t = 2;
    double old_res = 0.0;
    double new_res = 0.0;
    do
    {
        old_res = new_res;
        new_res = log(t) * gamma(t);
        t++;
    } while(fabs(new_res - old_res) > epsilon);

    return new_res;
}



int main(int argc, char* argv[])
{
    if(check_input(argc, argv) != SUCCESS)
    {
        printf("invalid input!\n");
    }
    else
    {
        char* epsilon_without_zeros = del_zeros(argv[1]);
        double epsilon = atof(epsilon_without_zeros);
        
        printf("\n");
        printf("The result of the e lim: %lf\n", e_lim(epsilon));
        printf("The result of the pi lim: %lf\n", pi_lim(epsilon));
        printf("The result of the ln_2 lim: %lf\n", ln_2_lim(epsilon));
        printf("The result of the sqrt_2 lim: %lf\n", sqrt_2_lim(epsilon));
        printf("The result of the gamma lim: %lf\n", gamma_lim(epsilon));
        printf("\n");
        printf("The result of the e row: %lf\n", e_row(epsilon));
        printf("The result of the pi raw: %lf\n", pi_row(epsilon));
        printf("The result of the ln 2 row: %lf\n", ln_2_row(epsilon));
        printf("The result of the sqrt 2 row: %lf\n", sqrt_2_row(epsilon));
        printf("The result of the gamma row: %lf\n", gamma_row(epsilon));
        printf("\n");
        printf("The result of the e equation: %lf\n", e_equation(epsilon));
        printf("The result of the pi equation:%lf\n", pi_equation(epsilon));
        printf("The result of the ln 2 equation:%lf\n", ln_2_equation(epsilon));
        printf("The result of the sqrt 2 equation:%lf\n", sqrt_2_equation(epsilon));
        printf("The result of the gamma equation:%lf\n", gamma_equation(epsilon));


        
        

        
    }
}
