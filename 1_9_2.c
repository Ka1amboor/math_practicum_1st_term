#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

enum status_code
{
    SUCCESS = 0,
    ALLOCATION_PROBLEM = 1
};

int nearest_value(int a_value, int* array_b, int size_b)
{
    int nearest_value = INT_MAX;
    
    for(int i = 0; i < size_b; i++)
    {
        if(abs(array_b[i] - a_value) < abs(nearest_value - a_value))
        {
            nearest_value = array_b[i];
        }
    }
    return nearest_value;
}



enum status_code fill_array_c(int* array_a, int* array_b, int** array_c, int size_a, int size_b, int size_c) 
{
    *array_c = (int*)malloc(sizeof(int) * size_c);

    if(!(*array_c))
    {   
        return ALLOCATION_PROBLEM;
    }

    for ( int i = 0; i < size_a; i++)
    {
        (*array_c)[i] = array_a[i] + nearest_value(array_a[i], array_b, size_b);
    }

    return SUCCESS;
}

enum status_code fill_array_with_random_nums(int** array, int size)
{
    *array = (int*)malloc(sizeof(int) * size);

    if (!(*array))
    {
        return ALLOCATION_PROBLEM;
    }

    for (int i = 0; i < size; i++)
    {
        (*array)[i] = -1000 + rand() % (1000 + 1000 + 1);
    }

    return SUCCESS;
}

int main()
{
    srand(time(NULL));

    int* array_a = NULL;
    int* array_b = NULL;
    int* array_c = NULL;

    int size_a = 10 + rand() % (10000 - 10 + 1);
    int size_b = 10 + rand() % (10000 - 10 + 1);

//fill and print array_a
    enum status_code status_a = fill_array_with_random_nums(&array_a, size_a);

    switch(status_a)
    {
        case ALLOCATION_PROBLEM:
        
            printf("failed to allocate memory\n");
            break;

        case SUCCESS:

            printf("ARRAY_A\n");

            for (int i = 0; i < size_a; i++)
            {
                printf("%d ", array_a[i]);
            }
            printf("\n");
            break;

        default:

            printf("unknown problem\n");
            break;
    }
//fill and print array_b

enum status_code status_b = fill_array_with_random_nums(&array_b, size_b);

    switch(status_b)
    {
        case ALLOCATION_PROBLEM:

            printf("failed to allocate memory\n");
            free(array_a);
            break;

        case SUCCESS:

            printf("ARRRAY_B:\n");

            for(int i = 0; i < size_b; i++)
            {
                printf("%d ", array_b[i]);
            }
            printf("\n");
            break;

        default:

            printf("unlnown problem\n");
            break;
    }

//fill and print array_c

enum status_code status_c = fill_array_c(array_a, array_b, &array_c, size_a, size_b, size_a);

    switch(status_c)
    {
        case ALLOCATION_PROBLEM:

            printf("failed to allocate memory\n");
            free(array_a);
            free(array_b);
            break;

        case SUCCESS:

        printf("ARRAY_C\n");

        for (int i = 0; i < size_a; i++)
        {
            printf("%d ", array_c[i]);
        }
        printf("\n");
        break;

        default:

            printf("unknown problem\n");
            break;
    }

    free(array_a);
    free(array_b);
    free(array_c);
    return 0;
}
