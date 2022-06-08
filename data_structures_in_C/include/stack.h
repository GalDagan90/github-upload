#ifndef __STACK_H__
#define __STACK_H__

#include <stddef.h> /* size_t */
#include <string.h> /* memcpy, strcpy */

typedef struct stack_t stack_t;

/**
 * @Desctiption : creating new stack.
 * @Parameters : max number of element of data_size.
 * @Return value : the stack entitiy.
**/ 
stack_t *StackCreate(size_t capacity, size_t data_size);

/**
 * @Desctiption : destroy stack.
 * @Parameters : stack to destroy.
 * @Return value : void.
**/ 
void StackDestroy(stack_t *stack);

/**
 * @Desctiption : removes the top element of the stack.
 * @Parameters : stack.
 * @Return value : the element on top of the stack.
**/ 
void *StackPop(stack_t *stack);

/**
 * @Desctiption : addes new element to the top of the stack.
 * @Parameters : stack, date to be store.
 * @Return value : void.
**/ 
void StackPush(stack_t *stack, const void *data);

/**
 * @Desctiption : looks at the top element on the stack without removing it.
 * @Parameters : stack.
 * @Return value : the element on top of the stack.
**/ 
void *StackPeek(const stack_t *stack);

/**
 * @Desctiption : summerize the elements in the stack.
 * @Parameters : stack.
 * @Return value : number of element in the stack.
**/ 
size_t StackSize(const stack_t *stack);

/**
 * @Desctiption : finding if the stack is empty.
 * @Parameters : stack.
 * @Return value : if the stack is empty 0 else 1.
**/ 
int IsStackEmpty(const stack_t *stack);

/**
 * @Desctiption : the max amount of elemet that can be store in the stack.
 * @Parameters : stack.
 * @Return value : capacity of the stack.
**/ 
size_t StackCapacity(const stack_t *stack);

#endif /* __STACK_H__ */
