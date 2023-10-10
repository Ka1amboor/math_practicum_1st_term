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


void find_min_max(int arr[], int size, int *min, int *max)
{
    *min = arr[0];
    *max = arr[1];
    for (int i = 1; i < size; i ++)
    {
        if(arr[i] < *min)
        {
            *min = arr[i];
        }

        if(arr[i] > *max)
        {
            *max = arr[i];
        }
    }
}

int swap_min_and_max(int arr[], int size)
{
    int min = arr[0];
    int min_index = 0;
    int max = arr[0];
    int max_index = 0;

    for (int i = 1; i < size; ++i)
    {
        if(arr[i] < min)
        {
            min = arr[i];
            min_index = i;
        }
        if (arr[i] > max)
        {
            max = arr[i];
            max_index = i;
        }
    
    }
    arr[min_index] = max;
    arr[max_index] = min;
    return success;
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
    const int size = 10;
    int arr[size];
    for (int i = 0; i < size; i++)
    {
        arr[i] = rand()%(b - a + 1) + a;
    }

    printf("Generated array: ");
    for (int i = 0; i < size; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");

    int min, max;
    find_min_max(arr, size, &min, &max);
    printf("Minimal value: %d\n", min);
    printf("Maximum value: %d\n", max);
    swap_min_and_max(arr, size);
    printf("Modified array: ");
    for(int i = 0; i < size; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    

    return 0;
}