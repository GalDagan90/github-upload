/******************************************************************************
 * Author: Gal Dagan                                                         *
 * Reviewer: Navot Ya'ari                                                        *
 * Created: 03.09.21                                                     *
 *                                                                            *
 * Description:                                                               *
 * implementation of a calculator as a finite state machine.                 *
 *                                                                            *
 * Infinity Labs OL108                                                        *
 ******************************************************************************/

#ifndef __CALCULATOR_H_OL108_ILRD__
#define __CALCULATOR_H_OL108_ILRD__

typedef enum error_t
{
	SUCCESS,
	SYNTAX_ERROR,
	MATH_ERROR,
	SYSTEM_ERROR
} error_t;


/**
 * @Description: Calculates a mathmetical expression from a string. 
 * @Parameters: expression - a string containing the expression to calculate.
 *              Permitted symbols include decimal numbers (with or without a
 *              decimal point and/or an exponant (E or e)), hexadecimal numbers
 *              (starting with 0x or 0X, potentailly containing a decimal point
 *              and/or an exponant (p or P)), infinity (INF or INFINITY,
 *              disregarding case), not-a-number (NAN, disregarding case),
 *              operators ('+', '-', '*', '/', '^'), unary operators ('+', '-'),
 *              brackets ('()', '[]', '{}'), and whitespaces.
 *              error - A pointer to keep the error status (or NULL).
 * @Return: If successful, returns the result of the calculation and sets error
 *          to SUCCESS (if error is not NULL). If not, returns zero and updates
 *          error (if not NULL) based on the error that happened.
 * @Complexity: O(n) - string length.
**/
double Calculate(const char *expression, error_t *error);

#endif /* __CALCULATOR_H_OL108_ILRD__ */

