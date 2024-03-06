#include <stdio.h>
#include <stdlib.h>
#include "Structures.h"

// argv[1] - максимальный приоритет, argv[2] - файл с инструкциями, etc - заявки пользователей
int main(
        int argc,
        char* argv[]
        )
{
    if(argc < 3)
    {
        printf("invalid input\n");
        return INVALID_INPUT;
    }

    int max_priority = atoi(argv[1]);
    FILE* file_1 = fopen(argv[2], "r");
    if(!file_1)
    {
        printf("error with opening file\n");
        return OPENING_FILE_ERROR;
    }


    return SUCCESS;
}
