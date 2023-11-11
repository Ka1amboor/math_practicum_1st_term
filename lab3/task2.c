#include <stdio.h>
#include <stdlib.h>
#include <math.h>


enum status_code
{
    SUCCESS = 0,
    MEMORY_ERROR = 1,
    INVALID_PARAMETER = 2

};

typedef struct
{
    double* coordinates;

} Vector;

enum status_code norm_of_chebyshev(double* result, Vector* vector, int size)
{

    *result = 0.0;

    for (int i = 0; i < size; i++)
    {
        if (*result < fabs(vector->coordinates[i]))
        {
            *result = fabs(vector->coordinates[i]);
        }
    }
    return SUCCESS;


}

enum status_code p_norm(double* result, Vector* vector, int size, int p)
{
    *result = 0.0;

    if (p > 1)
    {
        return INVALID_PARAMETER;
    }
    for (int i = 0; i < size; i++)
    {
        *result += (vector->coordinates[i])*(vector->coordinates[i]);
    }
    *result = sqrt(*result);

    return SUCCESS;
}

enum status_code a_norm(int size, double*** result, double* res, Vector* vector)
{
    double** unite_matrix = (double**)malloc(sizeof(double*) * size);// create unite matrix
    if(!unite_matrix)
    {
        return MEMORY_ERROR;
    }
    for (int i = 0; i < size; i++)
    {
        unite_matrix[i]=(double*)malloc(sizeof(double)*size);
        if(!unite_matrix[i])
        {
            return MEMORY_ERROR;
        }
        for (int j = 0; j < size; j++)
        {
            if(j == i)
            {
                unite_matrix[i][j] = 1;
            }
            else
            {
                unite_matrix[i][j] = 0;
            }
        }
    }
    double* multiplication = (double*)malloc(sizeof(double) * size);
    if (!multiplication)
    {
        return MEMORY_ERROR;
    }
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            multiplication[i] += unite_matrix[i][j] * vector->coordinates[j];
        }
    }
    double square_sum = 0;
    for (int i = 0; i < size; i++)
    {
        square_sum += pow(res[i], 2);
    }
    *res = sqrt(square_sum);

    for(int i = 0; i < size; i++)
    {
        free(unite_matrix[i]);
    }
    free(unite_matrix);
    free(multiplication);
    return SUCCESS;
}





int main()
{
    Vector vector, vector1, vector2;
    int dimension = 3;
    double result = 0;
    double** m_result = NULL; 
    int p = 1;


    vector.coordinates = (double*)malloc(dimension * sizeof(double));
    vector.coordinates[0] = 0.0;
    vector.coordinates[1] = 4.0;
    vector.coordinates[2] = 3.0;

    vector1.coordinates = (double*)malloc(dimension * sizeof(double));
    vector1.coordinates[0] = 1.0;
    vector1.coordinates[1] = 6.0;
    vector1.coordinates[2] = 8.0;

    vector2.coordinates = (double*)malloc(dimension * sizeof(double));
    vector2.coordinates[0] = 1.0;
    vector2.coordinates[1] = 6.0;
    vector2.coordinates[2] = 8.0;
    
    enum status_code chebyshev_status = norm_of_chebyshev(&result, &vector, dimension);

    if(chebyshev_status == SUCCESS)
    {
        printf("The Chebyshev norm result: %lf\n", result);
    }
    else
    {
        printf("error\n");
    }

    free(vector.coordinates);

    enum status_code p_status = p_norm(&result, &vector, dimension, p);

    if(p_status == SUCCESS)
    {
        printf("The p norm result: %lf\n", result);
    }
    else
    {
        printf("ERROR DETECTED! invalid p!");
    }

    free(vector.coordinates);

    enum status_code a_status = a_norm(dimension, &m_result, &result, &vector); //int size, double*** result, double* res, Vector* vector)

    if (a_status == SUCCESS)
    {
        printf("Result of a norm: %lf\n", result);
    }
    else
    {
        printf("Error\n");
    }
    free(m_result);

    // enum status_code super_function

   




    return 0;
}
