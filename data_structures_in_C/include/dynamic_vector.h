#ifndef __DYNAMIC_VECTOR_H__
#define __DYNAMIC_VECTOR_H__

#include <stddef.h> /* size_t */

typedef struct vector_t d_vector_t;

/**
 * @Description: Creates an empty dynamic vector.
 * @Parameters: data_size - the size each element that will be in the vector.
 *              capacity - initial capacity (maximal numner of elements) of the
 *              vector.
 * @Return: A pointer to the new vector, or NULL if memory alocation failed.
**/
d_vector_t *VectorCreate(size_t data_size, size_t capacity);

/**
 * @Description: Destroys the dynamic vector.
 * @Parameters: vector - pointer to the vector.
 * @Return: void.
**/
void VectorDestroy(d_vector_t *vector);

/**
 * @Description: Adds a data element to the end of the vector.
 * @Parameters: vector - pointer to the vector.
 *              data - a pointer to the data element.
 * @Return: Zero if the push was successful, one if vector reallocation failed.
 * @Notes: If the size of the element is different from the data_size that was
 *         specified when calling VectorCreate, the behaviour is undefined.
**/
int VectorPushBack(d_vector_t *vector, const void *data);

/**
 * @Description: Removes the last element from the vector.
 * @Parameters: vector - pointer to the vector.
 * @Return: void.
**/
int VectorPopBack(d_vector_t *vector);

/**
 * @Description: Updates the data of an element in the vector.
 * @Parameters: vector - pointer to the vector.
 *              index - the index to update.
 *              data - a pointer to the new data element.
 * @Return: void.
 * @Notes: If the index points to a non-exiting element, the function does
 *         nothing.
 *         If the size of the element is different from the data_size that was
 *         specified when calling VectorCreate, the behaviour is undefined.
**/
void VectorSetData(d_vector_t *vector, int index, const void *data);

/**
 * @Description: Gets the data of an element in the vector.
 * @Parameters: vector - pointer to the vector.
 *              index - the index to update.
 * @Return: A void pointer to the data in the vector, or NULL if the index does 
 *          not point to a valid index.
**/
void *VectorGetData(const d_vector_t *vector, int index);

/**
 * @Description: Shrinks the vector capacity to the vector's size.
 * @Parameters: vector - pointer to the vector.
 * @Return: Zero if the shrink was successful, one if not.
**/
int VectorShrink(d_vector_t *vector);

/**
 * @Description: Resizes the vector's capacity.
 * @Parameters: vector - pointer to the vector.
 *              new_capacity - the vactors new capacity.
 * @Return: Zero if the resizing was successful, one if not.
 * @Notes: Resize cannot shrink the vector below its current size.
**/
int VectorResize(d_vector_t *vector , size_t new_capacity);

/**
 * @Description: Returns the size of the vector.
 * @Parameters: vector - pointer to the vector.
 * @Return: The size of the vector (number of elements in the vector).
**/
size_t VectorGetSize(const d_vector_t *vector);

/**
 * @Description: Returns the capacity of the vector.
 * @Parameters: vector - pointer to the vector.
 * @Return: The capacity (maximal number of elements) of the vector.
**/
size_t VectorGetCapacity(const d_vector_t *vector);

#endif /* __DYNAMIC_VECTOR_H__ */
