/****************************************************************************
*Author : Gal Dagan
*Date : 
*Reviewer :
*
*Description :
*
*Lab : ol108
****************************************************************************/
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "knight_tour.h"

#define UNUSED(x) (void)(x)
#define BOARD_LEN 8
#define BOARD_SIZE 64


static void PrintBoard(int arr[][BOARD_LEN]);

int main(void)
{
	cell_t start_point = {0, 4};
	cell_t result_path[BOARD_SIZE] = {0};
	cell_t *sol = NULL;
	int i = 0;
	int board[BOARD_LEN][BOARD_LEN] = {0};

	/*sol = KnightTourBrute(start_point, result_path);*/
	sol = KnightTourWarnsdorff(start_point, result_path);

	for (; i < BOARD_SIZE; ++i)
	{
		board[sol[i].row][sol[i].col] = i;
	}

	PrintBoard(board);
	
	return 0;
}

static void PrintBoard(int arr[][BOARD_LEN])
{
	int i = 0, j = 0;

	for (; i < BOARD_LEN; ++i)
	{
		for (j = 0; j < BOARD_LEN; ++j)
		{
			printf("%3d", arr[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}



