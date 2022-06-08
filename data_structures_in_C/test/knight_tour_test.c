/****************************************************************************
*Author : Gal Dagan
*Date : 30.09.21
*Reviewer : Ido Sapan
*
*Description : Implementation of knight's tour problem.
*
*Lab : OL108
****************************************************************************/
#include <assert.h> /* assert() */
#include <stdio.h>  /* printf() */
#include <stdlib.h> /* malloc(), free() */

#include "knight_tour.h"

#define BOARD_LEN 8
#define BOARD_SIZE 64

static void Test(void);
static void PrintBoard(cell_t *path);

int main(void)
{
    Test();

    return 0;
}


static void Test(void)
{
    cell_t start_point = {0, 4};
    cell_t *result_path = (cell_t *)malloc(BOARD_SIZE * sizeof(cell_t));
    
    assert(NULL != result_path);

    printf("Brute : \n");
    if(NULL != KnightTourBrute(start_point, result_path))
    {
        PrintBoard(result_path);
    }
    else
    {
        printf("None!\n");
    }
    
    printf("\nWarnsdorff : \n");
    if(NULL != KnightTourWarnsdorff(start_point, result_path))
    {
        PrintBoard(result_path);
    }
    else
    {
        printf("None!\n");
    }

    free(result_path);
    result_path = NULL;
}


static void PrintBoard(cell_t *path)
{
    unsigned int board[BOARD_SIZE] = {0};
    unsigned int i = 0;

    for (; i < BOARD_SIZE; ++i)
    {
        board[(path[i].row) * BOARD_LEN + path[i].col] = i;
    }

    for (i = 0; i < BOARD_LEN; ++i)
    {
        size_t j = 0;
        for (; j < BOARD_LEN; ++j)
        {
            printf("%3u", board[i * BOARD_LEN + j]);
        }
        printf("\n");
    }
}