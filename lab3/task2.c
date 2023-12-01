#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>
#include <limits.h>


enum status_code
{
    SUCCESS = 0,
    INVALID_PARAMETER = 1,
    MEMORY_ERROR = 2

};

typedef struct
{
    double* coordinates;

}Vector;


void print_vector(Vector vector, int dimension)
{
    for (int i = 0; i < dimension; i++)
    {
        printf("%lf", vector.coordinates[i]);

    }
    printf("\n");
}

enum status_code norm_of_chebyshev(double* result, Vector vector, int dimension)
{
    if(dimension < 1)
    {
        return INVALID_PARAMETER;
    }
    double maxi = vector.coordinates[0];

    for (int i = 0; i < dimension; i++)
    {
        if (vector.coordinates[i] > maxi)
        {
            maxi = vector.coordinates[i];
        }
    }
    *result = maxi;
    return SUCCESS;


}

enum status_code p_norm(double* result, Vector vector, int dimension, int p)
{
    if (p < 1 || dimension <= 0)
    {
        return INVALID_PARAMETER;
    }
    double sum = 0.0;

    for (int i = 0; i < dimension; i++)
    {
        sum += pow(fabs(vector.coordinates[i]), p);
    }
    *result = pow(sum, 1.0/ p);

    return SUCCESS;
}

enum status_code a_norm(double* result, Vector vector, int dimension)
{
    if (dimension <= 0)
    {
        return INVALID_PARAMETER;
    }
    int unite_matrix[dimension][dimension];

    for (int i = 0; i < dimension; i++)
    {
        for(int j = 0; j < dimension; j++)
        {
            if (i == j)
            {
                unite_matrix[i][j] = 1;
            }
            else
            {
                unite_matrix[i][j] = 0;
            }
        }
    }

    double res[dimension];
    double sum = 0.0;

    for (int i = 0; i < dimension; i++)
    {
        for (int j = 0; j < dimension; j++)
        {
            sum += unite_matrix[i][j] * vector.coordinates[j];
        }
        res[i] = sum;
    }
    double pow_res = 0.0;
    for (int i = 0; i < dimension; i++)
    {
        pow_res += pow(res[i], 2);
    }
    *result = sqrt(pow_res);
    return SUCCESS;


}

enum status_code super_function
(
    double* result, Vector* result_vector, int dimension, int p,
    enum status_code (*norm_of_chebyshev)(double*, Vector, int),
    enum status_code (*p_norm)(double*, Vector, int, int),
    enum status_code (*a_norm)(double*, Vector, int),
    int count, ...
)
{

    va_list(ptr);
    va_start(ptr, count);

    double maxi_res[3];
    maxi_res[0] = INT_MIN;
    maxi_res[1] = INT_MIN;
    maxi_res[2] = INT_MIN;

    for (int i = 0; i < count; i++)//vectors
    {
        Vector vector = va_arg(ptr, Vector);//get every vector
        
        double result_[3];//find answer for vector
        
        if (norm_of_chebyshev(&(result_[0]), vector, dimension) != SUCCESS)
        {
            return INVALID_PARAMETER;
        }

        if (p_norm(&(result_[1]), vector, dimension, p) != SUCCESS)
        {
            return INVALID_PARAMETER;
        }

        if (a_norm(&(result_[2]), vector, dimension) != SUCCESS)
        {
            return INVALID_PARAMETER;
        }
        for (int j = 0; j < 3; j++)
        {
            if(result_[j] > maxi_res[j])
            {
                maxi_res[j] = result_[j];
                result_vector[j] = vector;//vector
                result[j] = maxi_res[j];//answer value

            }
        }
    }
    return SUCCESS;


}

int main()
{
    int p = 1;
    int dimension = 3;
    Vector vector1, vector2, vector3;

    vector1.coordinates = (double*)malloc(sizeof(double) * dimension);
    if (vector1.coordinates == NULL)
    {
        printf("failed to allocate memory\n");
        return MEMORY_ERROR;
    }
    vector1.coordinates[0] = 0.1;
    vector1.coordinates[1] = 0.5;
    vector1.coordinates[2] = 3.2;

    vector2.coordinates = (double*)malloc(sizeof(double) * dimension);
    if (vector2.coordinates == NULL)
    {
        printf("failed to allocate memory\n");
        return MEMORY_ERROR;
    }
    vector2.coordinates[0] = 0.1;
    vector2.coordinates[1] = 2.5;
    vector2.coordinates[2] = 3.2;

    vector3.coordinates = (double*)malloc(sizeof(double) * dimension);
    if (vector3.coordinates == NULL)
    {
        printf("failed to allocate memory\n");
        return MEMORY_ERROR;
    }
    vector3.coordinates[0] = 0.5;
    vector3.coordinates[1] = 2.5;
    vector3.coordinates[2] = 3.0;

    double result[3];
    Vector result_vector[3];
    int count = 3;

    super_function(result, result_vector, dimension, p, norm_of_chebyshev, p_norm, a_norm, count, vector1, vector2, vector3);

    for(int i = 0; i < count; i++)
    {
        printf("For norm number %d the longest vector is:\n", i + 1);
        print_vector(result_vector[i], dimension);
        printf("value of vector %lf\n", result[i]);
    }
    free(vector1.coordinates);
    free(vector2.coordinates);
    free(vector3.coordinates);

   
    return 0;
}
