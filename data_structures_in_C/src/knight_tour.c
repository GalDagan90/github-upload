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
#include <time.h>   /* time_t, time() */

#include "knight_tour.h"
#include "bit_array.h" /* bit_array API */

#define BOARD_LEN 8
#define BOARD_SIZE 64
#define KNIGHT_MOVES 8
#define NONE (-1)
#define MAX_RUN_TIME 300

typedef enum knight_tour_status
{
	SUCCESS = 0,
	PATH_FAILURE,
	TIMES_UP
}tour_status_t;

/****************************************************************************
*                      Static Function Declaration                          *
*****************************************************************************/
static int RecSolKnightTour(int cell, bit_array_t *board, 
                            int counter, cell_t *res_path, time_t start_time);
static void  WarnsSol(int row, int col, bit_array_t *board, cell_t *res_path);
static void InitBoard(cell_t start_point, int *start_cell, 
                        bit_array_t *board, cell_t *res_path);
static int HasNextMove(int *cell_index, bit_array_t *board);
static int NumOptionalMoves(int cell_index, bit_array_t *board);
static int IsEmpty(int cell_index, bit_array_t *board); 
static cell_t GetCell(int row, int col);  
static int GetBoardIndex(int row, int col);
static void SetRowAndCol(int index, int *row, int *col);
static void InitLUT();  
static void SetRowAndCol(int index, int *row, int *col);
static int IsTimeUp(time_t start_time);
static int IsBoardFull(bit_array_t board);

/****************************************************************************
*                                   LUTs                                    *
*****************************************************************************/
static int g_knight_row_moves[KNIGHT_MOVES] = {-2,-1, 1, 2, 2, 1,-1,-2};
static int g_knight_col_moves[KNIGHT_MOVES] = { 1, 2, 2, 1,-1,-2,-2,-1};
static int g_board_move[BOARD_SIZE][KNIGHT_MOVES] = {0};

/***************************************************************************/

cell_t *KnightTourBrute(cell_t start_point, cell_t *result_path)
{
    bit_array_t board = 0;
    int start_cell = 0;
    tour_status_t status = PATH_FAILURE;

    assert(NULL != result_path);

    InitBoard(start_point, &start_cell, &board, result_path);
    status = RecSolKnightTour(start_cell, &board, 1, result_path, time(NULL));
    
    return ((SUCCESS == status &&IsBoardFull(board)) ? result_path : NULL);
}

cell_t *KnightTourWarnsdorff(cell_t start_point, cell_t *result_path)
{
    bit_array_t board = 0;
    int start_cell = 0;

    assert(NULL != result_path);

    InitBoard(start_point, &start_cell, &board, result_path);
    WarnsSol(start_point.row, start_point.col, &board, result_path);

    return ((IsBoardFull(board)) ? result_path : NULL);
} 
/**************************************************************************
 *                      Static Functions                                  *
 **************************************************************************/
static void WarnsSol(int row, int col, bit_array_t *board, cell_t *res_path)
{
    int board_runner = 1;
    int cell_index = GetBoardIndex(row, col);

    for (; board_runner < BOARD_SIZE && 
    	   SUCCESS == HasNextMove(&cell_index, board); ++board_runner)
    {
        SetRowAndCol(cell_index, &row, &col);
        res_path[board_runner] = GetCell(row, col);   
    }
}                        

static int HasNextMove(int *cell_index, bit_array_t *board)
{
    int min_moves = KNIGHT_MOVES + 1;
    int best_move = NONE; 
    int best_cell = 0;
    int knight_runner = 0;

    for (; knight_runner < KNIGHT_MOVES; ++knight_runner)
    {
        int next_cell = g_board_move[*cell_index][knight_runner]; 
        
        if (NONE != next_cell && IsEmpty(next_cell, board) 
            && NumOptionalMoves(next_cell, board) < min_moves)
        {
            best_move = knight_runner;
            best_cell = next_cell;
            min_moves = NumOptionalMoves(next_cell, board);
        }
    }

    if (NONE == best_move)
    {
        return PATH_FAILURE;
    }

    *cell_index = best_cell;
    SetBit(board, *cell_index, ON);

    return SUCCESS;
}

static int NumOptionalMoves(int cell_index, bit_array_t *board)
{
    int knight_runner = 0;
    int legel_move_counter = 0;
    
    for (;knight_runner < KNIGHT_MOVES; ++knight_runner)
    {
        int next_cell = g_board_move[cell_index][knight_runner];
        
        if (NONE != next_cell && IsEmpty(next_cell, board))
        {
            ++legel_move_counter;
        }
    }

    return legel_move_counter;
}

static int RecSolKnightTour(int cell, bit_array_t *board, 
                            int counter, cell_t *res_path, time_t start_time)
{
    int knight_runner = 0;
    int row = 0;
    int col = 0;

    if (counter == BOARD_SIZE)
    {
        return SUCCESS;
    }
	
    if (IsTimeUp(start_time))
    {
        return TIMES_UP;
    }

    for (; knight_runner < KNIGHT_MOVES; ++knight_runner)
    {
        int next_cell = g_board_move[cell][knight_runner];
        
        if (NONE != next_cell && IsEmpty(next_cell, board))
        {
            SetBit(board, next_cell, ON);

        	if (SUCCESS == RecSolKnightTour(next_cell, board, counter + 1, 
            						  res_path, start_time))
            {
                SetRowAndCol(next_cell, &row, &col);
                res_path[counter] = GetCell(row, col);
                
                return SUCCESS;
            }
            
            SetBit(board, next_cell, OFF);
        }
    }
	
    return PATH_FAILURE;
}                            

static int IsEmpty(int cell_index, bit_array_t *board)
{
    bit_status_t status = ON;

    if (cell_index < BOARD_SIZE)
    {
        status = GetVal(*board, cell_index);   
    }
    
    return (OFF == status);
}

static cell_t GetCell(int row, int col)
{
    cell_t cell = {0,0};

    cell.row = (unsigned int)row;
    cell.col = (unsigned int)col;

    return cell;
}

static int GetBoardIndex(int row, int col)
{
    return (row * BOARD_LEN + col);
}

static void SetRowAndCol(int index, int *row, int *col)
{
    *col = index % BOARD_LEN;
    *row = index / BOARD_LEN;
}

static int IsInBoard(int row, int col)
{
    return ((row >= 0 && row < BOARD_LEN && col >= 0
            && col < BOARD_LEN ));
}

static void InitLUT()
{
    static int is_init = 0;
    int board_runner = 0;
    int knight_runner = 0;
    int row = 0;
    int col = 0;

    if (1 == is_init)
    {
        return;
    }

    is_init = 1;
    
    for (; board_runner < BOARD_SIZE; ++board_runner)
    {
        SetRowAndCol(board_runner, &row, &col);
        
        for (knight_runner = 0; knight_runner < KNIGHT_MOVES; ++knight_runner)
        {
            int next_row = row + g_knight_row_moves[knight_runner];
            int next_col = col + g_knight_col_moves[knight_runner];
            
            if (IsInBoard(next_row, next_col))
            {
                g_board_move[board_runner][knight_runner] = 
                							GetBoardIndex(next_row, next_col); 
            }
            else
            {
                g_board_move[board_runner][knight_runner] = NONE; 
            }
        }
    }
}

static void InitBoard(cell_t start_point, int *start_cell, 
                        bit_array_t *board, cell_t *res_path)
{
    InitLUT(board);
    *start_cell = GetBoardIndex((int)start_point.row, (int)start_point.col);
    SetBit(board, *start_cell, ON);
    res_path[0] = start_point;
}

static int IsTimeUp(time_t start_time)
{
	return (time(NULL) > start_time + MAX_RUN_TIME);
}

static int IsBoardFull(bit_array_t board)
{
    return (BOARD_SIZE == CountOn(board));
}
