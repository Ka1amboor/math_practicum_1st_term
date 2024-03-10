#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "Structures.h"

typedef struct Binary_Heap
{
    Application** array;
    int capacity;
    int size;

}Binary_Heap;

Binary_Heap* create_binary_heap()
{
    Binary_Heap* new_binary_heap = (Binary_Heap*)malloc(sizeof(Binary_Heap));
    if(!new_binary_heap)
    {
        return NULL;
    }

    new_binary_heap->array = (Application**)malloc(10 * sizeof(Application*));
    if(new_binary_heap->array == NULL)
    {
        free(new_binary_heap);
        return NULL;
    }

    new_binary_heap->capacity = 10;
    new_binary_heap->size = 0;


    return new_binary_heap;
}

void swap(Application** array, int index_1, int index_2)
{
    Application* tmp = array[index_1];
    array[index_1] = array[index_2];
    array[index_2] = tmp;
}

void shift_up(Binary_Heap* binary_heap, int index)
{
    int parent = (index - 1) / 2;

    while(index > 0 && ((binary_heap->array[index]->priority_of_app > binary_heap->array[parent]->priority_of_app) ||
            (binary_heap->array[index]->priority_of_app == binary_heap->array[parent]->priority_of_app &&
            strcmp(binary_heap->array[index]->time_of_receipt, binary_heap->array[parent]->time_of_receipt) < 0)))
    {
        swap(binary_heap->array, index, parent);
        index = parent;
        parent = (index - 1) / 2;
    }
}

status_code insert_in_binary_heap(Binary_Heap* binary_heap, Application* application)
{
    if(binary_heap->size == binary_heap->capacity)
    {
        binary_heap->capacity *= 2;
        binary_heap->array = (Application**)realloc(binary_heap->array, binary_heap->capacity * sizeof(Application*));
        if(!binary_heap->array)
        {
            binary_heap->capacity /= 2;
            free(binary_heap->array);
            free(binary_heap);
            return MEMORY_ERROR;
        }
    }

    binary_heap->array[binary_heap->size] = application;
    shift_up(binary_heap, binary_heap->size);
    binary_heap->size++;


    return SUCCESS;

}

void shift_down(Binary_Heap* binary_heap, int index)
{
    int parent = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if(left < binary_heap->size &&
    binary_heap->array[left]->priority_of_app > binary_heap->array[parent]->priority_of_app)
    {
        parent = left;
    }

    if(right < binary_heap->size &&
    binary_heap->array[right]->priority_of_app > binary_heap->array[parent]->priority_of_app)
    {
        parent = right;
    }

    if(parent != index)
    {
        swap(binary_heap->array, index, parent);
        shift_down(binary_heap, parent);
    }
}

Application* extract_max_binary_heap(Binary_Heap* binary_heap)
{
    if(binary_heap->size <= 0)
    {
        return NULL;
    }

    Application* deleted_application = binary_heap->array[0];
    binary_heap->array[0] = binary_heap->array[binary_heap->size - 1];
    binary_heap->size--;

    shift_down(binary_heap, 0);

    return deleted_application;
}

Application* find_max_binary_heap(Binary_Heap* binary_heap)
{
    if(binary_heap->size <= 0)
    {
        return NULL;
    }

    Application* maxi_application = binary_heap->array[0];

    return maxi_application;
}

void destroy_binary_heap(Binary_Heap* binary_heap)
{
    for(int i = 0; i < binary_heap->size; i++)
    {
        free_application(binary_heap->array[i]);
    }
    free(binary_heap->array);
    free(binary_heap);
}

void merge_binary_heaps_without_destruction(Binary_Heap* heap_1, Binary_Heap* heap_2)
{
    while(heap_2->size > 0)
    {
        insert_in_binary_heap(heap_1, extract_max_binary_heap(heap_2));
    }
}

void merge_binary_heaps_with_destruction(Binary_Heap* heap_1, Binary_Heap* heap_2)
{
    for(int i = 0; i < heap_2->size; i++)
    {
        insert_in_binary_heap(heap_1, heap_2->array[i]);
    }

    free(heap_2->array);
    free(heap_2);
    heap_2 = NULL;
}


