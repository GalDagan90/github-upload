/****************************************************************************
*Author : Gal Dagan
*Date : 
*Reviewer :
*
*Description :
*
*Lab : ol108
****************************************************************************/
#ifndef __HASH_TABLE_H_OL108_ILRD__
#define __HASH_TABLE_H_OL108_ILRD__

#include <stddef.h> /* size_t */


typedef struct hash_table hash_table_t;

/**
 * @Description: Pointer to a function that performs an operation on the value
 *               of a data element.
 * @Parameters: data - pointer to the data element, param - additional parameters
 * @Return: Zero if the operation was successful, non-zero if not.
**/
typedef int (*hash_table_oper_func_t)(void *data, void *param);

/**
 * @Description: Pointer to a function that compares the data of one data element
 *               with data of another void pointer.
 * @Parameters: Two void pointers to the data to compare.
 * @Return: One if they are equal, zero if not.
**/
typedef int (*hash_is_match_t)(const void *data, const void *to_comp);

/**
 * @Description: Computes hash code from data.
 * @Parameters: void pointer to data.
 * @Return: the computed hash code.
**/
typedef size_t (*hash_func_t)(void *data);

/*****************************************************************************/

/**
 * @Description: Creates an empty Hash Table.
 * @Parameters: hash_capacity - number of cells (unique keys) in the table.
 *				is_match - function to compare the data of different data elements.
 *				hash_func - function to calculate hash code from data.
 * @Return: a pointer to the new table.
 * @Complexity: O(k) - k is number of cells in the table.
 * @Note: size of 0 is not allowed.
**/
hash_table_t *HashTableCreate(size_t hash_size, hash_is_match_t is_match, 
							hash_func_t hash_func);

/**
 * @Description: Destroys the given hash table (but not the data).
 * @Parameters: table - a pointer to the hash table.
 * @Return: void.
 * @Complexity: O(n + k) - n is the number of entries occupied in the hash table, 
 *						   k is number of cells in the table.
**/
void HashTableDestroy(hash_table_t *table);

/**
 * @Description: Inserts data element with given data to the table.
 * @Parameters: table - pointer to the table
 *				data - pointer to the data of the data element to be inserted.
 * @Return: 0 if inserted successfuly, 1 if failed because of duplication, 2 if
 *			allocation failed.
 * @Notes: duplication - same data element already exists in the hash table (same
 *			data in the same cell).
 * @Complexity: average - O(n / k), worst - O(n)
**/
int HashTableInsert(hash_table_t *table, const void *data);

/**
 * @Description: Removes data element with given data from the table.
 * @Parameters: table - pointer to the table
 *				data - pointer to the data of the data element to be removed.
 * @Return: The data of the removed element.
 * @Notes: If the data is not in the hash table, NULL will be returned.
 * @Complexity: average - O(n / k), worst - O(n)
**/
void *HashTableRemove(hash_table_t *table, const void *data);

/**
 * @Description: Gets the size of the hash table.
 * @Parameters: table - a pointer to the table.
 * @Return: The size of the table (number of elements in the table).
 * @Complexity: O(n + k)
**/
size_t HashTableGetSize(const hash_table_t *table);

/**
 * @Description: Checks if the hash table is empty.
 * @Parameters: table - a pointer to the table.
 * @Return: One if it is empty, zero if not.
 * @Complexity: O(k)
**/
int HashTableIsEmpty(const hash_table_t *table);

/**
 * @Description: finds the given data in the table.
 * @Parameters: table - a pointer to the hash table 
 *				data - void pointer containing the value to find.
 * @Return: pointer to the data if found,  NULL otherwise.
 * @Complexity: average - O(n / k), worst - O(n)
**/
void *HashTableFind(const hash_table_t *table, const void *data);

/**
 * @Description: Run an operation on each data element in the hash table.
 * @Parameters: table - a pointer to the hash table.
 *              oper_func - a pointer to a function to operate on the data.
 *				params - a pointer to the function parameters.
 * @Return: Zero if all operations were successful, or the return value of
 *          operation_func if not.
 * @Notes: The function returns immediately if operation_func fails.
 * @Complexity: O(n + k)
**/
int HashTableForEach(hash_table_t *table, hash_table_oper_func_t oper_func, void *param);

/* Advanced Functions */

/**
 * @Description: Gets the load factor of the hash table.
 * @Parameters: table - a pointer to the table.
 * @Return: The load factor.
 * @Complexity: O(n + k)
**/
/*double HashTableLoad(hash_table_t *table);*/

/**
 * @Description: Gets the standard deviation of the hash table.
 * @Parameters: table - a pointer to the table.
 * @Return: The standard deviation of the table.
 * @Complexity: O(n + k)
**/
/*double HashTableSTD(hash_table_t *table);*/

#endif /* __HASH_TABLE_H_OL108_ILRD__ */

