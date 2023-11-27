#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

enum ERRORS
{
    success = 0,
    error = 1
};

void random_fill_array(int arr[], int size, int a, int b)
{
    for(int i = 0; i < size; i++)
    {
        arr[i] = a + rand() % (b - a + 1);
    }
}


void swap(int* min, int* max)
{
    int tmp = *max;
    *max = *min;
    *min = tmp;
}

void find_min_max_elem_swap(int arr[], int size, int* min_result, int* max_result)
{
    int min = arr[0];
    int min_index = 0;
    int max = arr[0];
    int max_index = 0;

    for(int i = 0; i < size; i++)
    {
        if(arr[i] < min)
        {
            min = arr[i];
            min_index = i;
        }

        if(arr[i] > max)
        {
            max = arr[i];
            max_index = i;
        }
    }

    *min_result = min;
    *max_result = max;
    
    swap(&arr[min_index], &arr[max_index]);
}

bool check_my_string(char* str)
{   
    int length = strlen(str);
    int i = 0;
    if (str[0] == '-' || str[0] == '+') i++;

    for (; i < length; i ++)
    {
        if(str[i] < '0' || str[i] > '9')
        {
            return false;
        }
    }
    return true;
}


int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        printf("Error! Few arguments, please enter a and b\n");
        return error;
    }
    if(!(check_my_string(argv[1])) || !(check_my_string(argv[2])))
    {
        printf("Invalid input error\n");
        return error;
    }

    int a = atoi(argv[1]);
    int b = atoi(argv[2]);

    if (a >= b)
    {
        printf("Input error!\n");
        return error;
    }

    srand(time(NULL));
    int size = 10;
    int arr[size];
    
    random_fill_array(arr, size, a, b);

    printf("Generated array: ");
    for (int i = 0; i < size; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");

    int min, max;
    find_min_max_elem_swap(arr, size, &min, &max);
    printf("Min value: %d\n", min);
    printf("Max value: %d\n", max);
    printf("Modified array: ");
    for(int i = 0; i < size; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
    return 0;
}
