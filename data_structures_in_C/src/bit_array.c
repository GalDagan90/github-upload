/******************************************************************
* Author: Gal Dagan
* Date: 26.07.21
* Reviewer: Albert Teverovsky
*
* Description: Implementation of bitarry data stucture
*
* InfintyLabs OL108
*******************************************************************/

#include "bit_array.h"

#define CHAR_BIT 8
#define WORD_SIZE sizeof(size_t) * CHAR_BIT
#define ZERO_IN_ASCII '0'
#define NULL_CHAR '\0'

void SetAll(bit_array_t *bit_arr)
{
	*bit_arr = 0xFFFFFFFFFFFFFFFF;
}

void ResetAll(bit_array_t *bit_arr)
{
	*bit_arr = 0;
}

void ToString(bit_array_t bit_arr, char *str)
{
	char *str_runner = str + WORD_SIZE;
	bit_array_t i = 0;
	
	*str_runner = NULL_CHAR;
	
	for (str_runner -= 1; i < WORD_SIZE; ++i, --str_runner, bit_arr >>= 1)
	{
		*str_runner = (bit_arr & 1) + ZERO_IN_ASCII;
	}
}

void SetOn(bit_array_t *bit_arr, unsigned int bit_pos)
{
	bit_array_t mask = 1;
	
	if (bit_pos < 64)
	{
		*bit_arr |= (mask << bit_pos);
	}
}

void SetOff(bit_array_t *bit_arr, unsigned int bit_pos)
{
	bit_array_t mask = 1;
	
	if (bit_pos < 64)
	{
		*bit_arr |= (mask << bit_pos);
		*bit_arr ^= (mask << bit_pos);
	}
}

void SetBit(bit_array_t *bit_arr, unsigned int bit_pos, bit_status_t status)
{
	 if (OFF == status)
	 {
	 	SetOff(bit_arr, bit_pos);
	 }
	 else
	 {
	 	SetOn(bit_arr, bit_pos);
	 }
}

bit_status_t GetVal(bit_array_t bit_arr, unsigned int bit_pos)
{
	bit_array_t mask = 1;
	
	return (0 != (bit_arr & (mask << bit_pos)))? 1 : 0;	
}

void RotR(bit_array_t *bit_arr, unsigned int num)
{
	*bit_arr = (*bit_arr >> num) | (*bit_arr << (WORD_SIZE - num));
}

void RotL(bit_array_t *bit_arr, unsigned int num)
{
	*bit_arr = (*bit_arr << num) | (*bit_arr >> (WORD_SIZE - num));
}

void Flip(bit_array_t *bit_arr, unsigned int bit_pos)
{
	bit_array_t mask = 1; 
	
	if (bit_pos < 64)
	{
		*bit_arr ^= (mask << bit_pos);
	}
}

unsigned int CountOn(bit_array_t bit_arr)
{
    int counter = 0;

    for (; 0 != bit_arr; bit_arr &= (bit_arr - 1), ++counter) 
    {
        /* Empty loop */
    }

    return counter;
}

unsigned int CountOff(bit_array_t bit_arr)
{
	return WORD_SIZE - CountOn(bit_arr);	
}

bit_array_t Mirror(bit_array_t bit_arr)
{
	bit_arr = (((bit_arr & 0xaaaaaaaaaaaaaaaa) >> 1) | ((bit_arr & 0x5555555555555555) << 1));
	bit_arr = (((bit_arr & 0xcccccccccccccccc) >> 2) | ((bit_arr & 0x3333333333333333) << 2));
	bit_arr = (((bit_arr & 0xf0f0f0f0f0f0f0f0) >> 4) | ((bit_arr & 0x0f0f0f0f0f0f0f0f) << 4));
	bit_arr = (((bit_arr & 0xff00ff00ff00ff00) >> 8) | ((bit_arr & 0x00ff00ff00ff00ff) << 8));
	bit_arr = (((bit_arr & 0xffffffffffffffff) >> 16) | ((bit_arr & 0xffffffffffffffff) << 16));
	
	return((bit_arr >> 32) | (bit_arr << 32));
}

