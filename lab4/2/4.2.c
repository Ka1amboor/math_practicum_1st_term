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

int main(int argc, char* argv[])
{   
    int j;
    int mass_count = 26;
    Mas* all_mass[mass_count];
    for(int i = 65; i < 91; i++)
    {
        all_mass[i-65] = create_mass(rand() % 20, (char)i);
        if(all_mass[i-65] == NULL)
        {
            printf("failed to allocate memory\n");
            free_all(all_mass, mass_count);
            return memory_error;
        }
        printf("name of mass: %c\n", all_mass[i]->name);
    }


    return 0;

}
