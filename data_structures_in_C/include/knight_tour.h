/****************************************************************************
*Author : Gal Dagan
*Date : 30.09.21
*Reviewer : Ido Sapan
*
*Description : Implementation of knight's tour problem.
*
*Lab : OL108
****************************************************************************/
#ifndef __KNIGHT_TOUR_H_OL108_ILRD__
#define __KNIGHT_TOUR_H_OL108_ILRD__

typedef struct cell
{
	unsigned int row: 3;
	unsigned int col: 3;
} cell_t;

/**
 * @Description: Find knight tour from start_point using recursive backtracking
 * @Parameters: start_point - starting cell (row, column)
 * 				result_path - array of size 64. contains the sequence of cells 
 * 							  in the tour.
 * @Return: NULL if fails to find a path, result_path otherwise.
 * @Note: the board is size 8x8. row, colum indexes are 0-7.
 * 		the search for path is failed if time exceeds five minutes.
**/
cell_t *KnightTourBrute(cell_t start_point, cell_t *result_path);

/**
 * @Description: Find knight tour from start_point using Warnsdorff algorithm
 * @Parameters: start_point - starting cell
 * 				result_path - array of size 64. 
 * 				contains the sequence of cells in the tour.
 * @Return: NULL if fails to find a path, result_path otherwise.
 * @Note: the board is size 8x8. row, colum indexes are 0-7.
 * 		the search for path is failed if time exceeds five minutes.
**/
cell_t *KnightTourWarnsdorff(cell_t start_point, cell_t *result_path);

#endif /* __KNIGHT_TOUR_H_OL108_ILRD__ */
