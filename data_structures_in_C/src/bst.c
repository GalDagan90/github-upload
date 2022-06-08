/****************************************************************************
* Author: Gal Dagan
* Date: 30.08.21
* Reviewer: Aharon Luzon
*
* Description: Implementation of Binary Search Tree (BST)
*
* InfintyLabs OL108
*****************************************************************************/
#include <stdlib.h> /* malloc(), free() */
#include <assert.h>	/* assert()	*/

#include "bst.h"

enum status
{
	SUCCESS,
	FAILURE
};

typedef enum child_pos
{
	LEFT,
	RIGHT,
	CHILDREN_NUM
} child_pos_t;

typedef struct bst_node
{
    struct bst_node *children[CHILDREN_NUM];
    struct bst_node *parent;
    void *data;
} bst_node_t;

struct bst
{   
    bst_node_t dummy;
    bst_cmp_t cmp_func;
};

/************** Static Function Declaration **********************************/
static bst_iter_t InitNewIter(bst_iter_t new_iter, void *data);
static bst_iter_t ChooseDirection(int cmp_res, bst_iter_t curr);
static bst_iter_t FindIter(bst_t *bst, void *data);
static void LinkIters(bst_t *bst, bst_iter_t parent, bst_iter_t child);
static bst_iter_t LeftMostChild(bst_iter_t iter);
static bst_iter_t RightMostChild(bst_iter_t iter);
static bst_iter_t GetLeftParent(bst_iter_t iter);
static bst_iter_t GetRightParent(bst_iter_t iter);
static bst_iter_t GetLeft(bst_iter_t iter);
static bst_iter_t GetRight(bst_iter_t iter);
static bst_iter_t GetParent(bst_iter_t iter);
static int SizeCounter(void *data, void *param);
static bst_iter_t GetDummyFromIter(bst_iter_t iter);
static void RemoveLeaf(bst_iter_t to_remove);
static void RemoveSingleParent(bst_iter_t to_remove);
static void RemoveFullParent(bst_iter_t to_remove);
static child_pos_t WhereIsMyChild(bst_iter_t parent, bst_iter_t child);
static bst_iter_t GetChildOfIter(bst_iter_t iter);
static int IsIterLeaf(bst_iter_t iter);
static int IsIterSingleParent(bst_iter_t iter);

/*****************************************************************************/

bst_t *BSTCreate(bst_cmp_t cmp_func)
{
	bst_t *new_bst = (bst_t *)malloc(sizeof(bst_t));
	if (NULL == new_bst)
	{
		return NULL;
	}
	
	new_bst->cmp_func = cmp_func;
	new_bst->dummy.children[LEFT] = NULL;
	new_bst->dummy.children[RIGHT] = NULL;
	new_bst->dummy.parent = NULL;
	new_bst->dummy.data = NULL;
	
	return new_bst;
}

void BSTDestroy(bst_t *bst)
{
	assert(NULL != bst);
	
	while (!BSTIsEmpty(bst))
	{
		BSTRemove(BSTBegin(bst));
	}
	
	free(bst);
	bst = NULL;
}

bst_iter_t BSTInsert(bst_t *bst, const void *data)
{
	bst_iter_t new_iter = {NULL};
	bst_iter_t parent_iter = {NULL};
	
	assert(NULL != bst);
	
	new_iter.node = (bst_node_t *)malloc(sizeof(bst_node_t));
	if (NULL == new_iter.node)
	{
		return new_iter;
	}
	
	new_iter = InitNewIter(new_iter, (void *)data);
	parent_iter = FindIter(bst, (void *)data);
	LinkIters(bst, parent_iter, new_iter);
	
	return new_iter;
}

void *BSTRemove(bst_iter_t to_remove)
{
	void *data = NULL;
	bst_iter_t dummy = {NULL};
	
	assert(NULL != to_remove.node);
		
	dummy = GetDummyFromIter(to_remove);
	if (!BSTIsSame(dummy, to_remove))
	{
		data = BSTGetData(to_remove);
		if (IsIterLeaf(to_remove))
		{
			RemoveLeaf(to_remove);
		}
		else if (IsIterSingleParent(to_remove))
		{
			RemoveSingleParent(to_remove);
		}
		else
		{
			RemoveFullParent(to_remove);
		}
	}
	
	return data;
}


bst_iter_t BSTFind(const bst_t *bst, const void *data)
{
	bst_iter_t res_iter = {NULL};
	bst_iter_t dummy = {NULL};
	
	assert(NULL != bst);	
	
	dummy = BSTEnd(bst);
	res_iter = FindIter((bst_t *)bst, (void *)data);
	if (BSTIsSame(dummy, res_iter) || 
		0 != bst->cmp_func(res_iter.node->data, data))
	{
		res_iter = dummy;
	}
	
	return res_iter;
}

bst_iter_t BSTBegin(const bst_t *bst)
{
	bst_iter_t iter = {NULL};
	
	assert(NULL != bst);
	
	iter = BSTEnd(bst);
	iter = LeftMostChild(iter);
	
	return iter;
}

bst_iter_t BSTEnd(const bst_t *bst)
{
	bst_iter_t iter = {NULL};
	
	assert(NULL != bst);
	
	iter.node = &((bst_t *)bst)->dummy;
	
	return iter;
}

bst_iter_t BSTPrev(bst_iter_t iter)
{
	bst_iter_t res_iter = {NULL};
		
	assert(NULL != iter.node);
	
	res_iter = GetLeft(iter);
	
	if (NULL != res_iter.node)
	{
		res_iter = RightMostChild(res_iter);
	}
	else
	{
		res_iter = GetLeftParent(iter);
	}
	
	return res_iter;
}

bst_iter_t BSTNext(bst_iter_t iter)
{
	bst_iter_t res_iter = {NULL};
		
	assert(NULL != iter.node);
	
	res_iter = GetRight(iter);
	
	if (NULL != res_iter.node)
	{
		res_iter = LeftMostChild(res_iter);
	}
	else
	{
		res_iter = GetRightParent(iter);
	}
	
	return res_iter;
	
}

void *BSTGetData(bst_iter_t iter)
{
	assert(NULL != iter.node);
	
	return iter.node->data;
}

size_t BSTSize(const bst_t *bst)
{
	size_t bst_size = 0;
	bst_callback_t cb_fnc = &SizeCounter;
	
	assert(NULL != bst);
	
	if (!BSTIsEmpty(bst))
	{
		BSTForEach(BSTBegin(bst), BSTEnd(bst), cb_fnc, &bst_size);
	}
	
	return bst_size;
}

int BSTIsEmpty(const bst_t *bst)
{
	assert(NULL != bst);
	
	return(NULL == bst->dummy.children[LEFT]);
}

int BSTForEach(bst_iter_t from, bst_iter_t to, bst_callback_t cb_fnc
		, void *param)
{
	int status = SUCCESS;
	bst_iter_t dummy = {NULL};
	
	assert(NULL != from.node);
	assert(NULL != to.node);
	assert(NULL != cb_fnc);
	
	dummy = GetDummyFromIter(from);
	
	for (; !(BSTIsSame(from, dummy)) && !(BSTIsSame(from, to)) 
		&& (SUCCESS == status);
		from = BSTNext(from))
	{
		status = cb_fnc(from.node->data, param);
	}
	
	return status;
}

int BSTIsSame(bst_iter_t iter1, bst_iter_t iter2)
{
	assert(NULL != iter1.node);
	assert(NULL != iter2.node);
	
	return (iter1.node == iter2.node);
}

/*****************************************************************************
*							Static functions								 *	
******************************************************************************/

static bst_iter_t GetDummyFromIter(bst_iter_t iter)
{
	for (; NULL != iter.node->parent; iter = GetParent(iter))
	{
		/* empty func */
	}
	
	return iter;
}

static void LinkIters(bst_t *bst, bst_iter_t parent, bst_iter_t child)
{
	if (BSTIsSame(parent, BSTEnd(bst)))
	{
		parent.node->children[LEFT] = child.node;	
	}	
	else
	{
		if (bst->cmp_func(parent.node->data, child.node->data) > 0)
		{	
			parent.node->children[LEFT] = child.node;
		}
		else if (bst->cmp_func(parent.node->data, child.node->data) < 0)
		{
			parent.node->children[RIGHT] = child.node;
		}
	}
	child.node->parent = parent.node;
}

static bst_iter_t FindIter(bst_t *bst, void *data)
{
	bst_iter_t parent = BSTEnd(bst);
	bst_iter_t curr = GetLeft(parent);
	int cmp_res = -1;
	
	while (NULL != curr.node && 0 != cmp_res)
	{
		cmp_res = bst->cmp_func(curr.node->data, data);
		parent.node = curr.node;
		curr = ChooseDirection(cmp_res, curr);
	}
	
	return parent;
}

static bst_iter_t InitNewIter(bst_iter_t new_iter, void *data)
{
	new_iter.node->children[LEFT] = NULL;
	new_iter.node->children[RIGHT] = NULL;
	new_iter.node->data = data;
	
	return new_iter;
}

static bst_iter_t ChooseDirection(int cmp_res, bst_iter_t curr)
{
	if (cmp_res > 0)
	{	
		curr = GetLeft(curr);
	}
	else
	{
		curr = GetRight(curr);
	}
	
	return curr;
}

static bst_iter_t LeftMostChild(bst_iter_t iter)
{
	for (; NULL != iter.node->children[LEFT]; iter = GetLeft(iter))
	{
		/* empty function */ 
	}
	
	return iter;
}

static bst_iter_t RightMostChild(bst_iter_t iter)
{
	for (; NULL != iter.node->children[RIGHT] ; iter = GetRight(iter))
	{
		/* empty function */ 
	}
	
	return iter;
}

static bst_iter_t GetLeftParent(bst_iter_t iter)
{
	bst_iter_t parent = GetParent(iter);
	
	for (; NULL != parent.node && GetRight(parent).node != iter.node; 
		iter.node = parent.node, parent = GetParent(iter))
	{
		/* empty loop */
	}
	
	return parent;
}

static bst_iter_t GetRightParent(bst_iter_t iter)
{
	bst_iter_t parent = GetParent(iter);
	
	for (; NULL != parent.node  && GetLeft(parent).node != iter.node; 
		iter.node = parent.node, parent = GetParent(iter))
	{
		/* empty loop */
	}
	
	return parent;
}


static bst_iter_t GetLeft(bst_iter_t iter)
{	
	iter.node = iter.node->children[LEFT];
	
	return iter;
}

static bst_iter_t GetRight(bst_iter_t iter)
{	
	iter.node = iter.node->children[RIGHT];
	
	return iter;
}

static bst_iter_t GetParent(bst_iter_t iter)
{
	iter.node = iter.node->parent;	
	
	return iter;
}

static int SizeCounter(void *data, void *param)
{
	int status = SUCCESS;
	/*bst_iter_t dummy = GetDummyFromIter(*((bst_iter_t *)data));*/
	
	if (NULL != data)
	{
		++(*(size_t *)param);
	}
	
	return status; 
}

static void RemoveFullParent(bst_iter_t to_remove)
{
	bst_iter_t prev = BSTPrev(to_remove);
	void *data = NULL;
	
	if (IsIterLeaf(prev))
	{
		to_remove.node->data = prev.node->data;
		RemoveLeaf(prev);
	}
	else
	{
		data = prev.node->data;
		RemoveSingleParent(prev);
		to_remove.node->data = data;
	}
}

static void RemoveSingleParent(bst_iter_t to_remove)
{
	bst_iter_t parent = {NULL};
	bst_iter_t del_child = {NULL};
	child_pos_t parent_pos = 0;
	child_pos_t child_pos = 0;
	
	parent = GetParent(to_remove);
	parent_pos = WhereIsMyChild(parent, to_remove);
	
	del_child = GetChildOfIter(to_remove);
	child_pos = WhereIsMyChild(to_remove, del_child);
	
	parent.node->children[parent_pos] = del_child.node;
	del_child.node->parent = parent.node;
	to_remove.node->children[child_pos] = NULL;
	to_remove.node->parent = NULL;
	
	free(to_remove.node);
}

static void RemoveLeaf(bst_iter_t to_remove)
{
	bst_iter_t parent = {NULL};
	child_pos_t pos = 0;
	
	parent = GetParent(to_remove);
	pos = WhereIsMyChild(parent, to_remove);
	to_remove.node->parent = NULL;
	parent.node->children[pos] = NULL;
	free(to_remove.node);
}

static bst_iter_t GetChildOfIter(bst_iter_t iter)
{
	if (NULL == GetLeft(iter).node)
	{
		iter = GetRight(iter);
	}
	else
	{
		iter = GetLeft(iter);
	}
	
	return iter;
}

static child_pos_t WhereIsMyChild(bst_iter_t parent, bst_iter_t child)
{	
	return (GetLeft(parent).node == child.node)? LEFT: RIGHT;	
}

static int IsIterSingleParent(bst_iter_t iter)
{
	return ( ((NULL == iter.node->children[LEFT]) && 
			  (NULL != iter.node->children[RIGHT])) || 
			  ((NULL != iter.node->children[LEFT]) && 
			  (NULL == iter.node->children[RIGHT]))  );
}

static int IsIterLeaf(bst_iter_t iter)
{
	return ((NULL == iter.node->children[LEFT]) && 
			(NULL == iter.node->children[RIGHT]));
}

