#include <stdio.h>
#include <stdlib.h>
#include "Structures.h"
#include <string.h>

//чтение информации об очереди и хранилище
status_code read_information_from_file(
        FILE* file_1,
        Structure_for_application* heap,
        Second_structure* structure
        )
{
    status_code status = SUCCESS;
    char _heap[32];
    char _structure[32];

    fscanf(file_1, "%s", _heap);

    if(strcmp(_heap, "BinaryHeap") == 0)
    {
        (*heap) = BINARY_HEAP;
    }
    else if(strcmp(_heap, "BinomialHeap") == 0)
    {
        (*heap) = BINOMIAL_HEAP;
    }
    else if(strcmp(_heap, "FibonacciHeap") == 0)
    {
        (*heap) = FIBONACCI_HEAP;
    }
    else if(strcmp(_heap, "SkewHeap") == 0)
    {
        (*heap) = SKEW_HEAP;
    }
    else if(strcmp(_heap, "LeftistHeap") == 0)
    {
        (*heap) = LEFTIST_HEAP;
    }
    else if(strcmp(_heap, "Treap") == 0)
    {
        (*heap) = TREAP;
    }
    else
    {
        status = INVALID_STRUCTURE;
    }

    fscanf(file_1, "%s", _structure);

    if(strcmp(_structure, "HashSet") == 0)
    {
        (*structure) = HASH_SET;
    }
    else if(strcmp(_structure, "DynamicArray") == 0)
    {
        (*structure) = DYNAMIC_ARRAY;
    }
    else if(strcmp(_structure, "BinarySearchTree") == 0)
    {
        (*structure) = BINARY_SEARCH_TREE;
    }
    else if(strcmp(_structure, "Trie") == 0)
    {
        (*structure) = TRIE;
    }
    else
    {
        status = INVALID_STRUCTURE;
    }

    printf("%s\n%s\n", _heap, _structure);

    return status;
}


// argv[1] - максимальный приоритет, argv[2] - файл с инструкциями, etc - заявки пользователей
int main(int argc,char* argv[])
{
    if(argc < 3)
    {
        printf("invalid input\n");
        return INVALID_INPUT;
    }

    int max_priority = atoi(argv[1]);
    printf("%d\n", max_priority);
    FILE* file_1 = fopen(argv[2], "r");
    if(!file_1)
    {
        printf("error with opening file\n");
        return OPENING_FILE_ERROR;
    }

    Structure_for_application heap;
    Second_structure structure;

    char start_modeling[20];
    int min_time_by_operator, max_time_by_operator, count_departments;
    double overload_coef;
    Department* departments;
    My_time* end_modeling = (My_time*)malloc(sizeof(My_time));
    if(!end_modeling)
    {
        printf("failed to allocate memory for end modeling\n");
        fclose(file_1);
        return MEMORY_ERROR;
    }

    //считали инф-ю из файла о структураъ и тд
    if(read_information_from_file(file_1, &heap, &structure) == INVALID_STRUCTURE)
    {
        printf("invalid structure in input file detected\n");
        fclose(file_1);
        free(end_modeling);
        return INVALID_STRUCTURE;
    }
    //считываем отсавшуюся информацию из файла
    fscanf(file_1, "%s", start_modeling);
    fscanf(file_1, "%d-%d-%dT%d:%d:%d", &end_modeling->year, &end_modeling->month, &end_modeling->day, &end_modeling->hour, &end_modeling->minute, &end_modeling->second);
    fscanf(file_1, "%d", &min_time_by_operator);
    fscanf(file_1, "%d", &max_time_by_operator);
    fscanf(file_1, "%d", &count_departments);

    if(count_departments  < 1 || count_departments > 100)
    {
        printf("detected invalid count of departments\n");
        free(end_modeling);
        fclose(file_1);
        return INVALID_INPUT;
    }


    departments = (Department*)malloc(sizeof(Department));
    if(!departments)
    {
        printf("failed to allocate memory for departments\n");
        free(end_modeling);
        fclose(file_1);
        return MEMORY_ERROR;

    }
    //считываем из файла для каждого отделения количество операторов для него
    for(int i = 0; i < count_departments; i++)
    {
        fscanf(file_1, "%d",&(departments[i].count_operators));
        if(departments[i].count_operators < 10 || departments[i].count_operators > 50)
        {
            printf("detected invalid count of operators\n");
            free(end_modeling);
            free(departments);
            fclose(file_1);
            return INVALID_INPUT;
        }
        departments[i].count_real_applications = 0;
        departments[i].count_free_applications = 0;
    }





    free(end_modeling);
    free(departments);
    fclose(file_1);

    return SUCCESS;
}
