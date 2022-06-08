/*******************************************************************
* Author: Gal Dagan
* Date: 30.08.21
* Reviewer: Aharon Luzon
*
* Description: Implementation of Binary Search Tree (BST)
*
* InfintyLabs OL108
********************************************************************/

#ifndef __BST_H_OL108_ILRD__
#define __BST_H_OL108_ILRD__

#include <stddef.h>	/* size_t	*/

typedef struct bst bst_t;

typedef struct bst_iter
{
	struct bst_node *node;
} bst_iter_t;


/**
 * @Description: Function pointer type that compares the data of two tree nodes.
 * @Parameters: Two void pointers to the data from two nodes.
 * @Return: Zero if they are equal, a negative number if lhs should be before rhs
 * and positive number if rhs should be before lhs.       
**/
typedef int (*bst_cmp_t)(const void *lhs, const void *rhs);

/**
 * @Description: Function pointer type that do some operation on the data 
 *of tree node.
 * @Parameters: void pointer to the data of a node.
 * @Return: 0 if the operation was successful, -1 otherwise.      
**/
typedef int (*bst_callback_t)(void *data, void *param);

/**
 * @Description: Creates a Bst determined by the cmp_node_t. 
 * @Parameters: cmp_func - A cmp_node_t function.
 * @Return: bst_t pointer creation was succesfull, NULL otherwise.
 * @Complexity: O(1)
**/
bst_t *BSTCreate(bst_cmp_t cmp_func);

/**
 * @Description: Destroy the given Bst. 
 * @Parameters: bst - A pointer to the Bst.
 * @Return: None.
 * @Notes n - is the number of nodes in Bst.
 * @Complexity: O(n)
**/
void BSTDestroy(bst_t *bst);

/**
 * @Description: Insert an node with a given data to the given Bst. 
 * @Parameters: bst - A pointer to the Bst, data = void pointer to the data.
 * @Return: poiner to the new node if insertion was succesfull, NULL otherwise.
 * @Notes n - is the number of nodes in Bst.
 * @Complexity: Average - O(logn) Worst - O(n)
**/
bst_iter_t BSTInsert(bst_t *bst, const void *data);

/**
 * @Description: Remove an node with a given data to the given Bst. 
 * @Parameters: to_remove- pointer to the given remove node.
 * @Return: poniter to the data of the removed node.
 * @Notes n - is the number of nodes in Bst.
 * @Complexity:  Average - O(logn) Worst - O(n)
**/
void *BSTRemove(bst_iter_t to_remove);

/**
 * @Description: find an node with a given data in the given Bst. 
 * @Parameters: bst- pointer to the given Bst, data - data of the node to find.
 * @Return: poniter to the found node, NULL otherwise.
 * @Notes n - is the number of nodes in Bst.
 * @Complexity:  Average - O(logn) Worst - O(n)
**/
bst_iter_t BSTFind(const bst_t *bst, const void *data);

/**
 * @Description: Return the first node.
 * @Parameters: bst- pointer to the given Bst.
 * @Return: pointer to the first node, NULL otherwise.
 * @Notes n - is the number of nodes in Bst.
 * @Complexity:  Average - O(logn) Worst - O(n)
**/
bst_iter_t BSTBegin(const bst_t *bst);

/**
 * @Description: Return the last node.
 * @Parameters: bst- pointer to the given Bst.
 * @Return: pointer to the first node, NULL otherwise.
 * @Notes n - is the number of nodes in Bst.
 * @Complexity:  Average - O(logn) Worst - O(n)
**/
bst_iter_t BSTEnd(const bst_t *bst);

/**
 * @Description: Return prev node in the bst.
 * @Parameters: bst- pointer to the given Bst.
 * @Return: pointer to the prev node, NULL otherwise.
 * @Notes n - is the number of nodes in Bst.
 * @Complexity:  Average - O(logn) Worst - O(n)
**/
bst_iter_t BSTPrev(bst_iter_t iter);

/**
 * @Description: Return next node in the bst.
 * @Parameters: bst-A pointer to the given Bst.
 * @Return: pointer to the next node, NULL otherwise.
 * @Notes n - is the number of nodes in Bst.
 * @Complexity:  Average - O(logn) Worst - O(n)
**/
bst_iter_t BSTNext(bst_iter_t iter);

/**
 * @Description: get data of the given bst node.
 * @Parameters: node-A pointer to the given bst node.
 * @Return: pointer to the data of a given data, NULL otherwise.
 * @Complexity: O(1)
**/
void *BSTGetData(bst_iter_t iter);

/**
 * @Description: Return the number of elemnts in the given Bst.
 * @Parameters: bst-A pointer to the given bst.
 * @Return: size of the bst.
 * @Complexity: O(n)
**/
size_t BSTSize(const bst_t *bst);

/**
 * @Description: Checks whether the given Bst is empty or not.
 * @Parameters: bst -A pointer to the given bst.
 * @Return: 1 if bst is empty, 0 otherwise.
 * @Complexity: O(1)
**/
int BSTIsEmpty(const bst_t *bst);

/**
 * @Description: Run an operation on each iterator in a section(from-to) of the bst.
 * @Parameters: from - the first iterator in the section.
 *              to - the first iterator after the section (the operation will 
 *              not be performed on its data).
 *              cb_fnc - a pointer to a function to operate on the data.
 *              The function should recieve a void pointer and params and return 
 *              zero if the operation was successful or non-zero if not.
 * @Return: Zero if all operations were successful, or the return value of
 *          operation_func if not.
 * @Notes: n - number of nodes between from - to.
 * @Complexity: O(n).
**/
int BSTForEach(bst_iter_t from, bst_iter_t to, bst_callback_t cb_fnc
		, void *param);

/**
 * @Description: Compare two iterators to check if they are identical.
 * @Parameters: iter1 - the first iterator.
 *              iter2 - the second iterator.
 * @Return: 1 if they are identical, 0 if not.
 * @Complexity: O(1).
**/
int BSTIsSame(bst_iter_t iter1, bst_iter_t iter2);


#endif /* __BST_H_OL108_ILRD__ */
