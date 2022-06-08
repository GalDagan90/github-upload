/*********************************************************
 *	heap_test.c
 *
 *	Author: Ido Sapan.
 *	Reviewer: Eyal Dafni.
 *	Date: 20.09.2021.
 *	Description:
 *		Testing of: HEAP API.
 *
 *	Infinity Labs OL108
 *
 *********************************************************/
#include <stdio.h> /* printf() */
#include <stdlib.h> /* malloc(), free() */
#include <assert.h> /* assert() */

#include "heap.h"
#include "dynamic_vector.h"
/************* defines *************************************/
#define UNUSED(x) ((void)(x))
#define TEST(test) (test(), puts(#test))
#define SIZE 10
#define SIZEXL 5000
#define PTR_DEREF(ptr) (*((void **) (ptr)))
/************* Globals *************************************/
heap_t *heap = NULL;
/************* enum *************************************/
typedef enum status
{
    SUCCESS = 0,
    FAILURE
} status_t;

typedef enum child
{
    LEFT = 1,
    RIGHT
} child_t;
/*************** struct ***************************************/
struct heap
{
	d_vector_t *d_vect;
	heap_cmp_t cmp_func;
};
/************* function declare *******************************/
int IsMatchInt(const void *lhs, const void *rhs);
int CmpInt(const void *lhs, const void *rhs);
void RandInit(int arr[], size_t size, int range, int offset);
/************* test-function declare *******************************/
void HeapCreateTest(void);
void HeapPushPopTest(void);
void HeapRemoveTest(void);
void HeapRemoveTestXL(void);
/*********** gray box function ***************************/
/**
 * @Description: Check if the heap is organized properly.
 * @Parameters: heap - a pointer to the tree.
 * @Return: The size of the heap if the heap organized properly,
 *          otherwise returns the index of the first element that
 *          violates the heap.
 * @Complexity: O(n).
**/
static int CheckHeapify(const heap_t *heap);
/************* gray box helper function *************************/
static int RecCheckHeapify(d_vector_t *d_vect, heap_cmp_t cmp_func, int index);
static void *GetData(d_vector_t *d_vect, int index);
static int GetChildIndex(int parent_index, child_t child);
/************* main ****************************************/
int main(int argc, char *argv[], char **envp)
{
    /*srand((unsigned int) time(NULL));*/
    srand(0);
    TEST(HeapCreateTest);
    TEST(HeapPushPopTest);
    TEST(HeapRemoveTest);
    TEST(HeapRemoveTestXL);

    UNUSED(argc);
    UNUSED(argv);
    UNUSED(envp);

    HeapDestroy(heap);
    heap = NULL;

    return (0);
}

/************* Implementation *********************************/
int IsMatchInt(const void *lhs, const void *rhs)
{
	assert(NULL != lhs);
	assert(NULL != rhs);

	return (*(int *)(lhs) == *(int *)(rhs));
}

int CmpInt(const void *lhs, const void *rhs)
{
	assert(NULL != lhs);
	assert(NULL != rhs);

	return (*(int *)(rhs) - *(int *)(lhs));
}

void RandInit(int arr[], size_t size, int range, int offset)
{
	size_t i = 0;

	for ( ; i < size; ++i)
	{
		arr[i] = (rand() % range) - offset;
	}
}
/************* test-function def *********************************/
void HeapCreateTest(void)
{
    assert(NULL == heap);
    heap = HeapCreate(CmpInt);
    assert(NULL != heap);
    assert(NULL == HeapPeek(heap));
    assert(HeapIsEmpty(heap));
    assert(0 == HeapSize(heap));
}

void HeapPushPopTest(void)
{
    int i = 0;
    int arr[SIZE];
    assert(NULL != heap);


    for (i = 0; SIZE > i; ++i)
    {
        arr[i] = i;
        assert(SUCCESS == HeapPush(heap, arr + i));
        assert(arr + i == HeapPeek(heap));
        assert(CheckHeapify(heap) == (int)HeapSize(heap));
    }


    for (i = SIZE - 1; 0 <= i; --i)
    {
        assert(arr + i == HeapPeek(heap));
        assert(arr + i == HeapPop(heap));
        assert(CheckHeapify(heap) == (int)HeapSize(heap));
    }

    assert(HeapIsEmpty(heap));
}

void HeapRemoveTest(void)
{
    int *arr = NULL;
    int i = 0;
    int test = SIZE + 1;

    assert(NULL != heap);

    arr = (int *)malloc(SIZE * sizeof(int));
    assert(NULL != arr);

    RandInit(arr, SIZE, SIZE, SIZE / 2);

    for (i = 0; SIZE > i; ++i)
    {
        assert(SUCCESS == HeapPush(heap, arr + i));
        assert(CheckHeapify(heap) == (int)HeapSize(heap));
    }

    for (i = 0; SIZE > i; ++i)
    {
        assert(arr[i] == *(int *)HeapRemove(heap, arr + i, IsMatchInt));
        assert(CheckHeapify(heap) == (int)HeapSize(heap));
        assert(SUCCESS == HeapPush(heap, arr + i));
        assert(CheckHeapify(heap) == (int)HeapSize(heap));
    }
    assert(NULL == HeapRemove(heap, &test, IsMatchInt));

    for (i = 0; SIZE > i; ++i)
    {
        assert(arr[i] == *(int *)HeapRemove(heap, arr + i, IsMatchInt));
        assert(CheckHeapify(heap) == (int)HeapSize(heap));
    }

    free(arr);
    arr = NULL;
}

void HeapRemoveTestXL(void)
{
    int *arr = NULL;
    int i = 0;
    int test = SIZEXL + 1;

    assert(NULL != heap);

    arr = (int *)malloc(SIZEXL * sizeof(int));
    assert(NULL != arr);

    RandInit(arr, SIZEXL, SIZEXL, SIZEXL / 2);

    for (i = 0; SIZEXL > i; ++i)
    {
        assert(SUCCESS == HeapPush(heap, arr + i));
        assert(CheckHeapify(heap) == (int)HeapSize(heap));
    }

    for (i = 0; SIZEXL > i; ++i)
    {
        assert(arr[i] == *(int *)HeapRemove(heap, arr + i, IsMatchInt));
        assert(CheckHeapify(heap) == (int)HeapSize(heap));
        assert(SUCCESS == HeapPush(heap, arr + i));
        assert(CheckHeapify(heap) == (int)HeapSize(heap));
    }
    assert(NULL == HeapRemove(heap, &test, IsMatchInt));

    for (i = 0; SIZEXL > i; ++i)
    {
        assert(arr[i] == *(int *)HeapRemove(heap, arr + i, IsMatchInt));
        assert(CheckHeapify(heap) == (int)HeapSize(heap));
    }

    free(arr);
    arr = NULL;
}
static int CheckHeapify(const heap_t *heap)
{
    assert(NULL != heap);
    assert(NULL != heap->d_vect);

    return (RecCheckHeapify(heap->d_vect, heap->cmp_func, 0));
}

static int RecCheckHeapify(d_vector_t *d_vect, heap_cmp_t cmp_func, int index)
{
    int size = 0;
    int left_index = GetChildIndex(index, LEFT);
    int right_index = GetChildIndex(index, RIGHT);
    int res_index = -1;

    assert(NULL != d_vect);

    size = (int)VectorGetSize(d_vect);
    if (size >= index)
    {
        return (size);
    }

    if ((size > left_index)
        && (0 > cmp_func(GetData(d_vect, left_index), GetData(d_vect, index))))
    {
        return (left_index);
    }

    if ((size > right_index)
        && (0 > cmp_func(GetData(d_vect, right_index), GetData(d_vect, index))))
    {
        return (right_index);
    }

    res_index = RecCheckHeapify(d_vect, cmp_func, left_index);
    if (size != res_index)
    {
        return (res_index);
    }

    return (RecCheckHeapify(d_vect, cmp_func, right_index));
}

static void *GetData(d_vector_t *d_vect, int index)
{
    assert(NULL != d_vect);

    return (PTR_DEREF(VectorGetData(d_vect, index)));
}

static int GetChildIndex(int parent_index, child_t child)
{
    assert(LEFT <= child);
    assert(RIGHT >= child);

    return (2 * parent_index + child);
}