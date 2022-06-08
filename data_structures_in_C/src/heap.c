/****************************************************************************
*Author : Gal Dagan
*Date : 22.09.21
*Reviewer : Ido Sapan
*
*Description : Implementation of heap data structure.
*
*Lab : ol108
****************************************************************************/

#include <stdlib.h>	/* malloc(), free() */
#include <assert.h> /* assert() */

#include "heap.h"
#include "dynamic_vector.h" /* dynamic_vector inner functions */

#define INIT_CAPACITY 1

typedef enum
{
    HEAP_SUCCESS,
    HEAP_FAILURE
}heap_status_t;

struct heap
{
    d_vector_t *heap_arr;
    heap_cmp_t cmp_func;
};

/******************************************************************************
*							Static function decleration						  *
******************************************************************************/
static void HeapifyUp(heap_t *heap, size_t curr_index);
static void HeapifyDown(heap_t *heap, size_t curr_index);
static void SwapData(heap_t *heap, size_t index_1, size_t index_2);
static size_t GetMinChildIndex(heap_t *heap, size_t index);
static size_t GetLeftIndex(size_t index);
static size_t GetRightIndex(heap_t *heap, size_t index);
static size_t FindDataInHeap(heap_t *heap, const void *to_del, 
                            is_match_heap_t is_match);

/*****************************************************************************/

heap_t *HeapCreate(heap_cmp_t cmp_func)
{
    heap_t *new_heap = NULL;

    assert(NULL != cmp_func);

    new_heap = (heap_t *)malloc(sizeof(heap_t));
    if (NULL == new_heap)
    {
        return NULL;
    }

    new_heap->heap_arr = VectorCreate(sizeof(void *), INIT_CAPACITY);
    if (NULL == new_heap->heap_arr)
    {
        free(new_heap);
        new_heap = NULL;

        return NULL;
    }

    new_heap->cmp_func = cmp_func;

    return new_heap;
}

void HeapDestroy(heap_t *heap)
{
    assert(NULL != heap);

    VectorDestroy(heap->heap_arr);
    heap->heap_arr = NULL;
    free(heap);
    heap = NULL;
}

int HeapPush(heap_t *heap, const void *data)
{
    assert(NULL != heap);
    assert(NULL != data);

    if (HEAP_SUCCESS != VectorPushBack(heap->heap_arr, &data))
    {
        return HEAP_FAILURE;
    }
   
    HeapifyUp(heap, HeapSize(heap) - 1);
    
    return HEAP_SUCCESS;
}

void *HeapPop(heap_t *heap)  
{
    size_t last_index = 0;
    void *popped_data = NULL;

    assert(NULL != heap);

    if (HeapIsEmpty(heap))
    {
        return NULL;
    }

    last_index = HeapSize(heap) - 1;
    SwapData(heap, 0, last_index);
    popped_data = *(void **)VectorGetData(heap->heap_arr, last_index);
    VectorPopBack(heap->heap_arr);
    
    if (HeapSize(heap) > 1)
    {
        HeapifyDown(heap, 0);
    }

    return popped_data;   
}

void *HeapPeek(const heap_t *heap) 
{
    void *data = NULL;

    assert(NULL != heap);

    if (!HeapIsEmpty(heap))
    {
        data = (*(void **)VectorGetData(heap->heap_arr, 0));
    }

    return data;
}

void *HeapRemove(heap_t *heap, const void *to_del, is_match_heap_t is_match)
{
    size_t to_del_index = 0;
    size_t last_index = 0;
    void *found_data = NULL;

    assert(NULL != heap);
    assert(NULL != is_match);

    last_index = HeapSize(heap) - 1;
    to_del_index = FindDataInHeap(heap, to_del, is_match);
    if (to_del_index <= last_index)
    {
        found_data = *(void **)VectorGetData(heap->heap_arr, to_del_index);
        SwapData(heap, to_del_index, last_index);
        VectorPopBack(heap->heap_arr);

        HeapifyDown(heap, to_del_index);
        HeapifyUp(heap, to_del_index);
    }

    return found_data;
}

int HeapIsEmpty(const heap_t *heap)
{
    assert(NULL != heap);

    return (0 == HeapSize(heap));
}

size_t HeapSize(const heap_t *heap)
{
    assert(NULL != heap);

    return (VectorGetSize(heap->heap_arr));
}

/******************************************************************************
*							Static functions            					  *
******************************************************************************/
static void HeapifyUp(heap_t *heap, size_t curr_index)
{
    size_t heap_size = HeapSize(heap);
    size_t parent_index = (curr_index - 1) / 2;
    void *curr_data = NULL;
    void * parent_data = NULL;

    if (heap_size  <= 1 || curr_index >= heap_size)
    {
        return;
    }

    curr_data  = *(void **)VectorGetData(heap->heap_arr, curr_index);
    parent_data = VectorGetData(heap->heap_arr, parent_index);
    
    while (NULL != parent_data && 0 != curr_index 
            && heap->cmp_func(curr_data, *(void **)parent_data) < 0)
    {
        SwapData(heap, curr_index, parent_index);
        curr_index = parent_index;

        if (parent_index > 0)
        {
            parent_index = (parent_index - 1) / 2;
            curr_data  = *(void **)VectorGetData(heap->heap_arr, curr_index);
            parent_data = VectorGetData(heap->heap_arr, parent_index);
        }
    }
}

static void HeapifyDown(heap_t *heap, size_t curr_index)
{
    size_t heap_size = HeapSize(heap);
    size_t min_index = GetMinChildIndex(heap, curr_index);
    void *curr_data = *(void **)VectorGetData(heap->heap_arr, curr_index);
    void *min_data = *(void **)VectorGetData(heap->heap_arr, min_index);
    
    if(heap_size <= 1)
    {
        return;
    }
    
    while (NULL != curr_data && (curr_index < heap_size / 2) && 
            (heap->cmp_func(curr_data, min_data) > 0))
    {
        SwapData(heap, curr_index, min_index);
        curr_index = min_index;
        if (curr_index < heap_size / 2)
        {
            curr_data = *(void **)VectorGetData(heap->heap_arr, curr_index);
            min_index = GetMinChildIndex(heap, curr_index);
            min_data = *(void **)VectorGetData(heap->heap_arr, min_index);
        }
    }
}

static void SwapData(heap_t *heap, size_t index_1, size_t index_2)
{
    void *data_1  = *(void **)VectorGetData(heap->heap_arr, index_1);
    void *data_2 = *(void **)VectorGetData(heap->heap_arr, index_2);

    VectorSetData(heap->heap_arr, index_2, &data_1);
    VectorSetData(heap->heap_arr, index_1, &data_2);
}

static size_t GetMinChildIndex(heap_t *heap, size_t index)
{
    size_t min_index = index;
    size_t left_index = GetLeftIndex(index);
    size_t right_index = GetRightIndex(heap, left_index);
    void *left_data = VectorGetData(heap->heap_arr, left_index); 
    void *right_data = VectorGetData(heap->heap_arr, right_index);

    if (NULL != left_data)
    {
        min_index = ((heap->cmp_func(*(void **)left_data, *(void **)right_data) < 0) ? 
                                    left_index: right_index);
    }
    
    return min_index;
}

static size_t GetLeftIndex(size_t index)
{
    return (index * 2 + 1);
}

static size_t GetRightIndex(heap_t *heap, size_t index)
{
    return ( (index >= (HeapSize(heap) - 1)) ? index : index + 1);
}

static size_t FindDataInHeap(heap_t *heap, const void *to_del, 
                            is_match_heap_t is_match)
{
    size_t runner = 0;
    size_t vector_size = VectorGetSize(heap->heap_arr);
    void *runner_data = NULL;
    int cmp_res = 0;

    for (; runner < vector_size &&  0 == cmp_res ;++runner)
    {
        runner_data = *(void **)VectorGetData(heap->heap_arr, runner);
        cmp_res = is_match(to_del, runner_data);    
    }

    return ((1 == cmp_res) ? (runner - 1) : vector_size) ;
}                            