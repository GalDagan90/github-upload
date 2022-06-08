/****************************************************************************
*Author : Gal Dagan
*Date : 10.09.21
*Reviewer :	Liel Sananes
*
*Description : Implementation of AVL trees with recursive functions
*
*Lab : ol108
****************************************************************************/
#include <stdlib.h>	/* malloc(), free() */
#include <assert.h> /* assert */

#include "avl.h"

#define UNUSED(x) ((void)(x))
#define Max(a,b) ( ((a) > (b)) ? (a) : (b) )
#define ABS(x) ( ((x) < 0) ? (-x) : (x) )

/*****************************************************************************/
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

typedef struct avl_node
{
	struct avl_node *child[CHILDREN_NUM];
	void *data;
	size_t height;
} avl_node_t;

struct avl
{
	avl_node_t *root;
	avl_cmp_t cmp_func;
};

typedef struct trav_tree
{
	int (*trav_f)(avl_node_t *, avl_callback_t, void *);	
}trav_tree_t;

/******************************************************************************
*							Static Function Decleration						  *
******************************************************************************/
static void ClearTree(avl_node_t *root);
static avl_node_t *CreateNewNode(const void *data);
static avl_node_t *InsertNodeRec(avl_node_t *node, avl_node_t *root, 
								avl_cmp_t cmp_f);
static avl_node_t *FindRec(avl_node_t *root, avl_cmp_t cmp_f, 
							const void *to_find);
static avl_node_t *RemoveNodeRec(avl_node_t *root, avl_cmp_t cmp_f, 
								const void *to_del);
static avl_node_t *RemoveScenario(avl_node_t *node_to_del, avl_cmp_t cmp_f);
static avl_node_t *FindSuccessor(avl_node_t *node);
static int NodeBalanceFactor(avl_node_t *root);
static avl_node_t *BalanceTree(avl_node_t *root);
static avl_node_t *RightRotate(avl_node_t *root);
static avl_node_t *LeftRotate(avl_node_t *root);
static void HeightUpdate(avl_node_t *node);
static int SizeCounter(void *data, void *param);
static size_t NodeHeight(avl_node_t *node);
static size_t MaxSubTreesHeight(avl_node_t *root);
static int IsNodeLeaf(avl_node_t *node);
static int InOrderTrav(avl_node_t *root, avl_callback_t cb_func, void *params);
static int PostOrderTrav(avl_node_t *root, avl_callback_t cb_func, void *params);
static int PreOrderTrav(avl_node_t *root, avl_callback_t cb_func, void *params);
			
/******************************************************************************
*									LUTs									  *
******************************************************************************/

trav_tree_t LUT_trav_g[] = {
	{PreOrderTrav},
	{InOrderTrav},
	{PostOrderTrav}
};

/******************************************************************************/
avl_t *AVLCreate(avl_cmp_t cmp_func)
{
	avl_t *new_root = NULL;
	
	assert(NULL != cmp_func);
	
	new_root = (avl_t *)malloc(sizeof(avl_t));
	if(NULL == new_root)
	{
		return NULL;
	}
	
	new_root->root = NULL;
	new_root->cmp_func = cmp_func;
	
	return new_root;
}

void AVLDestroy(avl_t *avl)
{
	assert(NULL != avl);
	
	ClearTree(avl->root);
	free(avl);
	avl = NULL;
}

int AVLInsert(avl_t *avl, const void *data)
{
	avl_node_t *new_node = NULL;
	
	assert(NULL != avl);
	assert(NULL != data);
	
	new_node = CreateNewNode(data);
	if (NULL == new_node)
	{
		return FAILURE;
	}
	if (AVLIsEmpty(avl))
	{
		avl->root = new_node;
	}
	else
	{
		avl->root = InsertNodeRec(new_node, avl->root, avl->cmp_func);
	}
	
	return SUCCESS;
}

void *AVLRemove(avl_t *avl, const void *to_remove)
{
	avl_node_t *node_to_del = NULL;
	void *data = NULL;
	
	assert(NULL != avl);
	assert(NULL != to_remove);
	
	node_to_del = FindRec(avl->root, avl->cmp_func, to_remove);
	if (NULL != node_to_del)
	{
		data = node_to_del->data;
		RemoveNodeRec(avl->root, avl->cmp_func, to_remove);	
		HeightUpdate(avl->root);
		if (ABS(NodeBalanceFactor(avl->root)) >= 2)
		{
			avl->root = BalanceTree(avl->root);
			HeightUpdate(avl->root);
		}
	}
	
	return data;
}

size_t AVLSize(const avl_t *avl)
{
	size_t avl_size = 0;
	avl_callback_t cb_func = &SizeCounter;
	
	assert(NULL != avl);
	
	if (!AVLIsEmpty(avl))
	{
		AVLForEach(avl, cb_func, &avl_size, IN_ORDER);
	}
	
	return avl_size;
}

int AVLIsEmpty(const avl_t *avl)
{
	assert(NULL != avl);
	
	return (NULL == avl->root);
}

void *AVLFind(const avl_t *avl, const void *to_find)
{	
	avl_node_t *found_node = NULL;
	void *found_data = NULL;
	
	assert(NULL != avl);
	
	found_node = FindRec(avl->root, avl->cmp_func, to_find);
	if (NULL != found_node)
	{
		found_data = found_node->data;
	}
	
	return found_data;
}

int AVLForEach(const avl_t *avl, avl_callback_t cb_func, void *params, 
				trav_order_t order)
{
	int status = SUCCESS;
	
	assert(NULL != avl);
	assert(NULL != cb_func);
		
	status = LUT_trav_g[order].trav_f(avl->root, cb_func, params);
	
	return status;
}

size_t AVLHeight(const avl_t *avl)
{
	size_t height = 0;
	
	assert(NULL != avl);
	
	if (!AVLIsEmpty(avl))
	{
		height = (avl->root->height);
	}
	
	return height;
}

/******************************************************************************
*								Static Functions							  *
******************************************************************************/
static void ClearTree(avl_node_t *root)
{
	if (NULL == root)
	{
		return;
	}
	
	ClearTree(root->child[LEFT]);
	ClearTree(root->child[RIGHT]);
	free(root);
	root = NULL;
}

static avl_node_t *CreateNewNode(const void *data)
{
	avl_node_t *node = (avl_node_t *)malloc(sizeof(avl_node_t));
	if (NULL != node)
	{
		node->child[LEFT] = NULL;
		node->child[RIGHT] = NULL;
		node->height = 0;
		node->data = (void *)data;
	}
	
	return node;
}

static avl_node_t *FindRec(avl_node_t *root, avl_cmp_t cmp_f, 
							const void *to_find)
{
	avl_node_t *found_node = NULL;
	
	if (NULL == root)
	{
		return NULL;
	}
	
	if (cmp_f(to_find, root->data) > 0)
	{
		found_node = FindRec(root->child[RIGHT], cmp_f, to_find);
	}
	else if (cmp_f(to_find, root->data) < 0)
	{
		found_node = FindRec(root->child[LEFT], cmp_f, to_find);
	}
	else
	{
		found_node = root;
	}
	
	return found_node;
}

static avl_node_t *InsertNodeRec(avl_node_t *node, avl_node_t *root, 
								avl_cmp_t cmp_f)
{	
	if (NULL == root)
	{
		return node;
	}
	
	if (cmp_f(node->data, root->data) > 0)
	{
		root->child[RIGHT] = InsertNodeRec(node, root->child[RIGHT], cmp_f);
	}
	else
	{
		root->child[LEFT] = InsertNodeRec(node, root->child[LEFT], cmp_f);
	}
	
	HeightUpdate(root);
	if (ABS(NodeBalanceFactor(root)) >= 2)
	{
		root = BalanceTree(root);
		HeightUpdate(root);
	}
	
	return root;
}

static avl_node_t *RemoveNodeRec(avl_node_t *root, avl_cmp_t cmp_f, 
								const void *to_del)
{	
	if (NULL == root)
	{
		return NULL;
	}
	
	if (cmp_f(to_del, root->data) > 0)
	{
		root->child[RIGHT] = RemoveNodeRec(root->child[RIGHT], cmp_f, to_del);
	}
	else if (cmp_f(to_del, root->data) < 0)
	{
		root->child[LEFT] = RemoveNodeRec(root->child[LEFT], cmp_f, to_del);
	}
	else
	{
		root = RemoveScenario(root, cmp_f);
		
		return root;
	}
	
	HeightUpdate(root);
	if (ABS(NodeBalanceFactor(root)) >= 2)
	{
		root = BalanceTree(root);
		HeightUpdate(root);
	}
	
	return root;
}

static avl_node_t *RemoveScenario(avl_node_t *node_to_del, avl_cmp_t cmp_f)
{
	avl_node_t *temp = NULL;
	
	if (NULL == node_to_del->child[LEFT])
	{	
		temp = node_to_del->child[RIGHT];
		free(node_to_del);
		return temp;
	}
	else if (NULL == node_to_del->child[RIGHT])
	{
		temp = node_to_del->child[LEFT];
		free(node_to_del);
		return temp;
	}
	else /* full parent */
	{
		temp = FindSuccessor(node_to_del->child[RIGHT]);
		node_to_del->data = temp->data;
		node_to_del->child[RIGHT] = RemoveNodeRec(node_to_del->child[RIGHT],
													cmp_f, temp->data);
	}
	
	return node_to_del;
}

static avl_node_t *FindSuccessor(avl_node_t *node)
{
	if (NULL == node->child[LEFT])
	{
		return node;
	}
	
	return FindSuccessor(node->child[LEFT]);
}

static int NodeBalanceFactor(avl_node_t *root)
{
	int left_sub_height = -1;
	int right_sub_height = -1;
	
	if (NULL != root->child[LEFT])
	{
		left_sub_height = NodeHeight(root->child[LEFT]);
	}
	if (NULL != root->child[RIGHT])
	{
		right_sub_height = NodeHeight(root->child[RIGHT]);
	}
	
	return ((int)(left_sub_height - right_sub_height));
}

static avl_node_t *BalanceTree(avl_node_t *root)
{
	int root_bf = NodeBalanceFactor(root);
	
	if (2 == root_bf)
	{
		if (-1 == NodeBalanceFactor(root->child[LEFT]))
		{
			root->child[LEFT] = LeftRotate(root->child[LEFT]);	
		}
		root = RightRotate(root);
	}
	else /* -2 == root_bf */
	{
		if (1 == NodeBalanceFactor(root->child[RIGHT]))
		{
			root->child[RIGHT] = RightRotate(root->child[RIGHT]);
		}
		root = LeftRotate(root);
	}
	
	return root;
}

static avl_node_t *RightRotate(avl_node_t *root)
{
	avl_node_t *rotated_root = root->child[LEFT];
	avl_node_t *temp = root->child[LEFT]->child[RIGHT];
	
	root->child[LEFT]->child[RIGHT] = root;
	root->child[LEFT] = temp;
	
	return rotated_root;
}

static avl_node_t *LeftRotate(avl_node_t *root)
{
	avl_node_t *rotated_root = root->child[RIGHT];
	avl_node_t *temp = root->child[RIGHT]->child[LEFT];
	
	root->child[RIGHT]->child[LEFT] = root;
	root->child[RIGHT] = temp;
	
	return rotated_root;
}

static size_t MaxSubTreesHeight(avl_node_t *root)
{
	return Max(NodeHeight(root->child[LEFT]), NodeHeight(root->child[RIGHT]));
}

static size_t NodeHeight(avl_node_t *node)
{
	if (NULL == node)
	{
		return 0;
	}
	
	return (node->height);
}

static int SizeCounter(void *data, void *param)
{
	int status = SUCCESS;
	
	if (NULL != data)
	{
		++(*(size_t *)param);
	}
	
	return status; 
}

static void HeightUpdate(avl_node_t *node)
{
	if (NULL == node)
	{
		return;
	}
	
	HeightUpdate(node->child[LEFT]);
	HeightUpdate(node->child[RIGHT]);
	node->height = IsNodeLeaf(node) ? 0 : (1 + MaxSubTreesHeight(node));
}

static int InOrderTrav(avl_node_t *root, avl_callback_t cb_func, void *params)
{
	int status = SUCCESS;
	
	if (NULL == root)
	{
		return SUCCESS;
	}
	
	if (FAILURE != status)
	{
		status = InOrderTrav(root->child[LEFT], cb_func, params);
	}
	if (FAILURE != status)
	{
		status = cb_func(root->data, params);
	}
	if (FAILURE != status)
	{
		status = InOrderTrav(root->child[RIGHT], cb_func, params);
	}
		
	return status;
}

static int PreOrderTrav(avl_node_t *root, avl_callback_t cb_func, void *params)
{
	int status = SUCCESS;
	
	if (NULL == root)
	{
		return SUCCESS;
	}
	
	status = cb_func(root->data, params);
	
	if (FAILURE != status)
	{
		status = PreOrderTrav(root->child[LEFT], cb_func, params);
	}
	if (FAILURE != status)
	{
		status = PreOrderTrav(root->child[RIGHT], cb_func, params);
	}

	return status;
}

static int PostOrderTrav(avl_node_t *root, avl_callback_t cb_func, void *params)
{
	int status = SUCCESS;
	
	if (NULL == root)
	{
		return SUCCESS;
	}
	
	if (FAILURE != status)
	{
		status = PostOrderTrav(root->child[LEFT], cb_func, params);
	}
	if (FAILURE != status)
	{
		status = PostOrderTrav(root->child[RIGHT], cb_func, params);
	}

	status = cb_func(root->data, params);
	
	return status;
}

static int IsNodeLeaf(avl_node_t *node)
{
	return ((NULL == node->child[LEFT]) && (NULL == node->child[RIGHT]));
}
