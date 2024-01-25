#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <limits.h>
#include <stdarg.h>

typedef enum
{
    success,
    memory_error,
    error_with_opening_file,
    invalid_input

}status_code;

status_code check_action(int action)
{
    if(action < 0 || action > 14)
    {
        return invalid_input;
    }

    return success;
}

typedef struct
{
    int* array;
    int capasity;
    int size;
    char name;

}Mas;

Mas* create_mass(int capasity, char name)
{
    Mas* new_mas = (Mas*)malloc(sizeof(Mas));
    if(!new_mas)
    {
        return NULL;
    }
    new_mas->array = (int*)malloc(sizeof(int)*capasity);
    if(!new_mas->array)
    {
        return NULL;
    }

    new_mas->capasity = capasity;
    new_mas->name = name;
    new_mas->size = 0;

    return new_mas;
}

void free_all(Mas** all_mass, int mass_count)
{
    for(int i = 0; i < mass_count; i++)
    {
        if(all_mass[i]->array != NULL)
        {
            free(all_mass[i]->array);
            all_mass[i]->capasity = 0;
            all_mass[i]->size = 0;
        }
        if(all_mass[i] != NULL)
        {
            free(all_mass[i]);
        }
    }
}
void print_action()
{
    printf("****************************************\n");
    printf("*     Please select an action           *\n");
    printf("*                                       *\n");
    printf("*   Enter <1> load a, in.txt            *\n");
    printf("*                                       *\n");
    printf("****************************************\n");
}
int main(int argc, char* argv[])
{   
    int action = 0;
    int flag = 1;
    char array_name;
    while(flag)
    {
        print_action();
        scanf("%d", &action);
        if(check_action(action) == invalid_input)
        {
            printf("invalid input\n");
            return invalid_input;
        }
        switch(action)
        {
            case 0:
                flag = 0;
                break;
            case 1:
                printf("enter the name of array: <A-Z>");
                if(scanf("%s", &array_name) != 1)
                {
                    printf("invalid input!\n");
                    return invalid_input;
                }

        }
    }
    


    return 0;

}
