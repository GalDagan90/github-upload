#ifndef __BIT_ARRAY_H__
#define __BIT_ARRAY_H__

#include <stddef.h> /* size_t */

typedef enum
{
	OFF,
	ON
} bit_status_t;

typedef size_t bit_array_t;

/**
 * @Description: Sets all the bits of the bit array to ON.
 * @Parameters: bit_arr - a pointer to the bit array.
 * @Return: void.
**/
void SetAll(bit_array_t *bit_arr); 

/**
 * @Description: Sets all the bits of the bit array to OFF.
 * @Parameters: bit_arr - a pointer to the bit array.
 * @Return: void.
**/
void ResetAll(bit_array_t *bit_arr);

/**
 * @Description: Writes the bit array to a string as zeros and ones.
 * @Parameters: bit_arr - the bit array.
 *              str - a pointer to a char array with at least 64 bytes.
 * @Return: void.
**/
void ToString(bit_array_t bit_arr, char *str); 

/**
 * @Description: Sets a specific bit in the bit array to ON.
 * @Parameters: bit_arr - a pointer to the bit array.
 *              bit_pos - the position of the bit (0-63).
 * @Notes: The function does nothing if bit_pos is larger than 63.
 * @Return: void.
**/
void SetOn(bit_array_t *bit_arr, unsigned int bit_pos); 

/**
 * @Description: Sets a specific bit in the bit array to OFF.
 * @Parameters: bit_arr - a pointer to the bit array.
 *              bit_pos - the position of the bit (0-63).
 * @Notes: The function does nothing if bit_pos is larger than 63.
 * @Return: void.
**/
void SetOff(bit_array_t *bit_arr, unsigned int bit_pos);

/**
 * @Description: Sets a specific bit in the bit array to a specified status.
 * @Parameters: bit_arr - a pointer to the bit array.
 *              bit_pos - the position of the bit (0-63).
 *              status - the status for the bit (OFF/ON).
 * @Notes: The function does nothing if bit_pos is larger than 63.
 *         Any status other than OFF will set the bit to ON.
 * @Return: void.
**/
void SetBit(bit_array_t *bit_arr, unsigned int bit_pos, bit_status_t status); 

/**
 * @Description: Return the value of a specific bit.
 * @Parameters: bit_arr - the bit array.
 *              bit_pos - the position of the bit (0-63).
 * @Return: The status of the bit (OFF/ON), or OFF if bit_pos is larger than 63.
**/
bit_status_t GetVal(bit_array_t bit_arr, unsigned int bit_pos); 

/**
 * @Description: Flips a specific bit (changes ON to OFF and OFF to ON).
 * @Parameters: bit_arr - a pointer to the bit array.
 *              bit_pos - the position of the bit (0-63).
 * @Notes: The function does nothing if bit_pos is larger than 63.
 * @Return: void.
**/
void Flip(bit_array_t *bit_arr, unsigned int bit_pos); 

/**
 * @Description: Create a mirror image of the bit array (from left to right).
 * @Parameters: bit_arr - the bit array.
 * @Return: The mirror image of the bit array.
**/
bit_array_t Mirror(bit_array_t bit_arr); 

/**
 * @Description: Rotates the bit array a specified number of bits to the right.
 * @Parameters: bit_arr - a pointer to the bit array.
 *              num - number of bits to rotate.
 * @Return: void.
**/
void RotR(bit_array_t *bit_arr, unsigned int num); 

/**
 * @Description: Rotates the bit array a specified number of bits to the left.
 * @Parameters: bit_arr - a pointer to the bit array.
 *              num - number of bits to rotate.
 * @Return: void.
**/
void RotL(bit_array_t *bit_arr, unsigned int num); 

/**
 * @Description: Counts the number of bits that are ON in the bit array.
 * @Parameters: bit_arr - the bit array.
 * @Return: The number of bits that are ON.
**/
unsigned int CountOn(bit_array_t bit_arr); 

/**
 * @Description: Counts the number of bits that are OFF in the bit array.
 * @Parameters: bit_arr - the bit array.
 * @Return: The number of bits that are OFF.
**/
unsigned int CountOff(bit_array_t bit_arr); 

#endif
