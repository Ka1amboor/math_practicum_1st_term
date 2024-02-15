
#include <stdio.h>
#include <stdlib.h>

typedef struct Heap
{
    int* array;
    int size;
    int capacity;

} Heap;

void print_heap(Heap* heap, int i, int level)
{
    if (i < heap->size) 
    {
        print_heap(heap, 2 * i + 2, level + 1);
        for (int j = 0; j < level; j++) 
        {
            printf("    ");
        }
        printf("%d\n", heap->array[i]);
        print_heap(heap, 2 * i + 1, level + 1);
    }
}

void swap(int* a, int* b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void max_heapify(Heap* heap, int i)
{
    int parent = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < heap->size && heap->array[left] > heap->array[parent])
    {
        parent = left;
    }

    if (right < heap->size && heap->array[right] > heap->array[parent])
    {
        parent = right;
    }

    if (parent != i)
    {
        swap(&heap->array[i], &heap->array[parent]);
        max_heapify(heap, parent);
    }
}

void insert(Heap* heap, int data)
{

    int i = heap->size;
    heap->array[i] = data;
    
    while (i > 0 && heap->array[i] > heap->array[(i - 1) / 2])
    {
        swap(&heap->array[i], &heap->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }

    heap->size++;

    max_heapify(heap, i); 
}


int delete_max(Heap* heap)
{
    int deleteItem;

	if (heap->size == 0) 
    {
		printf("\nHeap id empty.");
		return -999;
	}

	deleteItem = heap->array[0];

	heap->array[0] = heap->array[heap->size - 1];
	heap->size--;

	max_heapify(heap, 0);
	return deleteItem;
}

Heap* create_heap(int capacity, int* nums)
{
    Heap* heap = (Heap*)malloc(sizeof(Heap));
    if (!heap)
    {
        return NULL;
    }

    heap->array = (int*)malloc(sizeof(int) * capacity);
    if (!heap->array)
    {
        free(heap);
        return NULL;
    }

    heap->size = 0;
    heap->capacity = capacity;

    int i = 0;
    for (i; i < capacity; i++)
    {
        heap->array[i] = nums[i];
    }

    heap->size = i;
    
    i = (heap->size - 2) / 2;
    
    while (i >= 0)
    {
        max_heapify(heap, i);
        i--;
    }

    return heap;
}

void destroy_heap(Heap* heap)
{
    if(heap)
    {
        if(heap->array)
        {
            free(heap->array);
        }
        free(heap);
    }
}

Heap* make_heap(int capacity)
{
    Heap* new_heap = (Heap*)malloc(sizeof(Heap));
    if(!new_heap)
    {
        return NULL;
    }

    new_heap->array = (int*)malloc(sizeof(int) * capacity);
    new_heap->capacity = capacity;
    new_heap->size = 0;

    return new_heap;
}

Heap* merge_heaps_with_destruction(Heap* heap1, Heap* heap2)
{
    Heap* new_heap = make_heap(heap1->capacity + heap2->capacity);

    for(int i = 0; i < heap1->size; i++)
    {
        new_heap->array[i] = heap1->array[i];
    }

    for (int i = 0; i < heap2->size; i++)
    {
        new_heap->array[heap1->size + i] = heap2->array[i];
    }

    new_heap->size = heap1->size + heap2->size;

    max_heapify(new_heap, 0);
    destroy_heap(heap1);
    destroy_heap(heap2);

    return new_heap;
}

int main()
{
    int nums[10] = {1, 2, 3, 4, 5, 6, 7};
    int n[3] = {1, 2, 3};
    Heap* heap = create_heap(10, nums);
    Heap* h = create_heap(3, n);

    Heap* heap_w_d = merge_heaps_with_destruction(heap, h);
    print_heap(heap_w_d, 0, 0);
    destroy_heap(heap_w_d);

    // print_heap(heap, 0, 0);
    // printf("\n\n\n\n");
    // insert(heap, 9);
    // print_heap(heap, 0, 0);
    // delete_max(heap);
    // printf("\n\n\n\n");
    // print_heap(heap, 0, 0);
    // destroy_heap(heap);
    return 0;
}
