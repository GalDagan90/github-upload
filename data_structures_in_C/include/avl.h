/****************************************************************************
*Author : Gal Dagan
*Date : 10.09.21
*Reviewer :	Liel Sananes
*
*Description : Implementation of AVL trees with recursive functions
*
*Lab : ol108
****************************************************************************/
#ifndef __AVL_H_OL108_ILRD__
#define __AVL_H_OL108_ILRD__

#include <stddef.h>	/* size_t*/

typedef struct avl avl_t;

typedef enum
{
	PRE_ORDER,
	IN_ORDER,
	POST_ORDER
} trav_order_t;

/**
 * @Description: Function pointer type that compares the data of two nodes.
 * @Parameters: Two void pointers to the data from two nodes.
 * @Return: Zero if they are equal, a negative number if lhs should be before
 *          rhs, or a positive number if rhs should be before lhs.       
**/
typedef int (*avl_cmp_t)(const void *lhs, const void *rhs);

/**
 * @Description: Function pointer type to perform some operation on the data 
 *               of a node.
 * @Parameters: A void pointer to the data of a node, and a second void pointer
 *              to extra parameters that can be sent to AVLForEach.
 * @Return: Zero if the operation was successful, non-zero otherwise.      
**/
typedef int (*avl_callback_t)(void *data, void *param);

/******************************************************************************
 *                           FUNCTION DECLARATIONS                            *
 ******************************************************************************/

/**
 * @Description: Creates an AVL tree determined by the cmp_node_t. 
 * @Parameters: cmp_func - A avl_cmp_t function.
 * @Return: avl_t pointer if creation was succesfull, NULL otherwise.
 * @Complexity: O(1)
**/
avl_t *AVLCreate(avl_cmp_t cmp_func);

/**
 * @Description: Destroy the given AVL. 
 * @Parameters: avl - A pointer to the AVL.
 * @Return: None.
 * @Notes n - is the number of nodes in AVL.
 * @Complexity: O(n)
**/
void AVLDestroy(avl_t *avl);

/**
 * @Description: Insert an node with a given data to the given AVL. 
 * @Parameters: avl - A pointer to the AVL, data = void pointer to the data.
 * @Return: 0 if insertion was succesfull, non-zero otherwise.
 * @Notes n - is the number of nodes in AVL. 
 *		Balancing is performed when	necassery.
 * @Complexity: O(logn).
**/
int AVLInsert(avl_t *avl, const void *data);

/**
 * @Description: Remove a node with a given data to the given AVL. 
 * @Parameters: to_remove- pointer to the given remove data.
 * @Return: poniter to the data of the removed node.
 * @Notes n - is the number of nodes in AVL.
 *		Balancing is performed when	necassery.
 * @Complexity: O(logn).
**/
void *AVLRemove(avl_t *avl, const void *to_remove);

/**
 * @Description: Return the number of nodes in given AVL.
 * @Parameters: avl-A pointer to a given AVL.
 * @Return: size of the avl.
 * @Complexity: O(n)
**/
size_t AVLSize(const avl_t *avl);

/**
 * @Description: Checks whether the given AVL is empty or not.
 * @Parameters: avl -A pointer to an AVL.
 * @Return: 1 if bst is empty, 0 otherwise.
 * @Complexity: O(1)
**/
int AVLIsEmpty(const avl_t *avl);

/**
 * @Description: find an node with a given data in a given AVL. 
 * @Parameters: avl- pointer to AVL tree, to_find - data of the node to find.
 * @Return: poniter to the found data, NULL otherwise.
 * @Notes n - is the number of nodes in AVL.
 * @Complexity: O(logn).
**/
void *AVLFind(const avl_t *avl, const void *to_find);

/**
 * @Description: Run an operation on each node in a given avl.
 * @Parameters: avl - pointer to AVL tree.
 *              cb_fnc - a pointer to a function to operate on the data.
 *              The function should recieve a void pointer and params and return 
 *              zero if the operation was successful or non-zero if not.
 *				order - Traverse order (In/Pre/Post-order traversal).
 * @Return: Zero if all operations were successful, or the return value of
 *          operation_func if not.
 * @Notes: n - number of nodes in the AVL.
 * @Complexity: O(n).
**/
int AVLForEach(const avl_t *avl, avl_callback_t cb_func, void *params
		, trav_order_t order);

/**
 * @Description: Checks the height of a given AVL.
 * @Parameters: avl -A pointer to an AVL.
 * @Return: height of the AVL.
 * @Complexity: O(1)
**/
size_t AVLHeight(const avl_t *avl);

#endif /* __AVL_H_OL108_ILRD__ */

