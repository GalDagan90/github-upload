/******************************************************************************
 * Author: Gal Dagan                                                          *
 * Reviewer: Navot Ya'ari                                                     *
 * Created: 04.09.21                                                          *
 *                                                                            *
 * Description:                                                               *
 * implementation of a calculator as a finite state machine.                  *
 *                                                                            *
 * Infinity Labs OL108                                                        *
 ******************************************************************************/
 
#include <stdlib.h> /* malloc(), free(), strtod() */
#include <string.h>	/* strlen() 	*/
#include <stddef.h>	/* size_t		*/
#include <ctype.h> 	/* isspace() 	*/
#include <math.h>	/* pow()		*/
#include <assert.h>	/* assert() 	*/

#include "stack.h" /* stack functions */
#include "calculator.h"

#define UNUSED(x) ((void)(x))
#define NULL_CHAR '\0'
#define OPER_RANGE 256

/******************************************************************************
 *                                   ENUMS                                    *
 *****************************************************************************/

typedef enum state_id_t
{
	WAIT_FOR_NUM,
	WAIT_FOR_OPERATOR
} state_id_t;

typedef enum input_id_t
{
	ILLEGAL_INPUT,
	OPEN_BRACKET,
	PLUS,
	MINUS,
	TIMES,
	DIVISION,
	POWER,
	CLOSE_BRACKET,
	NUMBER
} input_id_t;

/******************************************************************************
 *                         			STRUCTS                         		  *
 *****************************************************************************/

typedef struct calculator
{
	stack_t *num_stack;
	stack_t *oper_stack;
}calc_t;

typedef struct input_type
{
	double num;
	char oper;
	input_id_t id;
} input_t;

typedef error_t (*calc_function)(double *, double *);
typedef error_t (*handler_function)(calc_t *calc, char *oper);

typedef input_t (* parse_fnc_t) (char **);
typedef void (* action_fnc_t) (input_t, calc_t *, error_t *);

typedef struct operators_callback
{
	calc_function calc_func;
	input_id_t priority;
} operator_t;

typedef struct handler
{
	handler_function handler_f;
} handler_t;

typedef struct state_t
{
	parse_fnc_t parse_fnc;
	action_fnc_t action_fnc;
} state_t;

/*****************************************************************************/
/*							Static Function declaration						 */
/*****************************************************************************/

static calc_t *CreateCalc(size_t stack_size);
static void InitOperLUT(operator_t *LUT_opers_g);
static void InitOperHandlers(handler_t *LUT_handlers_g);
static void DestroyCalc(calc_t *calc);
static double MainHandler(calc_t *calc, const char *str, error_t *error);

static input_t ParseNum(char **input_str);
static void ActionNum(input_t input, calc_t *calc, error_t *error);
static input_t ParseOper(char **input_str);
static void ActionOper(input_t input, calc_t *calc, error_t *error);
static void ChangeState(state_id_t *state, input_t input);
static error_t ErrorHandler(calc_t *calc, char *oper);
static error_t HandleOper(calc_t *calc, char *oper);
static error_t HandlePow(calc_t *calc, char *oper);
static error_t HandleOpenParan(calc_t *calc, char *oper);
static error_t HandleCloseParan(calc_t *calc, char *oper);
static int IsHigherPriority(char curr_oper, char oper);
static int IsMatchingOpenParan(char *oper_in_stack, char oper);
static error_t DoCalculations(calc_t *calc, char *oper, char *curr_oper);
static double *DoOneCalc(calc_t *calc, char *curr_oper, error_t *error);
static char *SkipWhiteSpace(char *runner);
static double GetFinalRes(calc_t *calc, error_t *error);
static void ErrorAction(calc_t *calc, error_t error, double *num1);

static input_id_t GetOperId(char oper);
static error_t GeneralErrorHandler(double *num1, double *num2);
static error_t Add(double *num1, double *num2);
static error_t Sub(double *num1, double *num2);
static error_t Multi(double *num1, double *num2);
static error_t Div(double *num1, double *num2);
static error_t Pow(double *num1, double *num2);
static error_t OpenParan(double *num1, double *num2);
static error_t CloseParan(double *num1, double *num2);

/*****************************************************************************
 *                             Global LUTS & variables                       *
 *****************************************************************************/

const state_t STATE_LUT[] = {
		{ParseNum, ActionNum},
		{ParseOper, ActionOper}
};

operator_t LUT_opers_g[OPER_RANGE] = { {NULL, 0} };
handler_t LUT_handlers_g[OPER_RANGE] = {{NULL}};

double error_num_g = 0.0;

/*****************************************************************************/

double Calculate(const char *expression, error_t *error)
{
	calc_t *calc = NULL;
	double res = 0.0;
	
	assert(NULL != expression);
	
	*error = SUCCESS;
	calc = CreateCalc(strlen(expression));
	if (NULL == calc)
	{
		return SYSTEM_ERROR;
	}
	
	res = MainHandler(calc, expression, error);
	DestroyCalc(calc);
	
	return res;
}

/*****************************************************************************/
/*								Static Functions 						     */
/*****************************************************************************/

static calc_t *CreateCalc(size_t stack_size)
{
	calc_t *calc = (calc_t *)malloc(sizeof(calc_t));
	if (NULL == calc)
	{
		return NULL;
	}
	
	calc->num_stack = StackCreate(stack_size, sizeof(double));
	if (NULL == calc->num_stack)
	{
		free(calc);
		calc = NULL;
		return NULL;
	}
	
	calc->oper_stack = StackCreate(stack_size, sizeof(char));
	if (NULL == calc->oper_stack)
	{
		StackDestroy(calc->num_stack);
		free(calc);
		calc = NULL;
		return NULL;
	}
	
	InitOperLUT(LUT_opers_g);
	InitOperHandlers(LUT_handlers_g);
	
	return calc;
}

static void InitOperHandlers(handler_t *LUT_handlers_g)
{
	
	LUT_handlers_g[ILLEGAL_INPUT].handler_f = ErrorHandler;	
	LUT_handlers_g[PLUS].handler_f = HandleOper;
	LUT_handlers_g[MINUS].handler_f = HandleOper;
	LUT_handlers_g[TIMES].handler_f = HandleOper;
	LUT_handlers_g[DIVISION].handler_f = HandleOper;
	LUT_handlers_g[POWER].handler_f = HandlePow;
	LUT_handlers_g[OPEN_BRACKET].handler_f = HandleOpenParan;
	LUT_handlers_g[CLOSE_BRACKET].handler_f = HandleCloseParan;	
}

static void InitOperLUT(operator_t *LUT_opers_g)
{
	LUT_opers_g[ILLEGAL_INPUT].calc_func = GeneralErrorHandler;
	LUT_opers_g[ILLEGAL_INPUT].priority = ILLEGAL_INPUT;
	
	LUT_opers_g[PLUS].calc_func = Add;
	LUT_opers_g[PLUS].priority = PLUS;
	
	LUT_opers_g[MINUS].calc_func = Sub;
	LUT_opers_g[MINUS].priority = PLUS;

	LUT_opers_g[TIMES].calc_func = Multi;
	LUT_opers_g[TIMES].priority = TIMES;

	LUT_opers_g[DIVISION].calc_func = Div;
	LUT_opers_g[DIVISION].priority = TIMES;

	LUT_opers_g[POWER].calc_func = Pow;
	LUT_opers_g[POWER].priority = POWER;
	
	LUT_opers_g[OPEN_BRACKET].calc_func = OpenParan;
	LUT_opers_g[OPEN_BRACKET].priority = OPEN_BRACKET;
	
	LUT_opers_g[CLOSE_BRACKET].calc_func = CloseParan;
	LUT_opers_g[CLOSE_BRACKET].priority = CLOSE_BRACKET;
}

static void DestroyCalc(calc_t *calc)
{
	assert(NULL != calc);
	
	StackDestroy(calc->num_stack);
	StackDestroy(calc->oper_stack);
	free(calc);
	calc = NULL;
}

static double MainHandler(calc_t *calc, const char *str, error_t *error)
{
	char *runner = (char *)str;
	state_id_t state = WAIT_FOR_NUM;
	double res = 0.0;
	input_t input = {0,0,0};
	
	while (SUCCESS == *error && NULL_CHAR != *runner)
	{
		input = STATE_LUT[state].parse_fnc(&runner);
		STATE_LUT[state].action_fnc(input, calc, error);
		ChangeState(&state, input);
	}
	
	if (SUCCESS == *error && NULL_CHAR == *runner)
	{
		res = GetFinalRes(calc, error);
	}
	
	return (SUCCESS == *error)? res: 0.0;
}

static double GetFinalRes(calc_t *calc, error_t *error)
{		
	while (!IsStackEmpty(calc->oper_stack))
	{
		char *curr_oper = (char *)StackPeek(calc->oper_stack);	
		double *num1 = DoOneCalc(calc, curr_oper, error);
		ErrorAction(calc, *error, num1);
	}
	
	if (IsStackEmpty(calc->num_stack))
	{
		*error = SYNTAX_ERROR; 
	}
	
	return (SUCCESS == *error)? *((double *)StackPop(calc->num_stack)): 0.0;
}

static void ErrorAction(calc_t *calc, error_t error, double *num1)
{
	if (SUCCESS == error)
	{
		StackPush(calc->num_stack, num1);
	}
	else
	{
		StackPush(calc->num_stack, &error_num_g);
	}
}

static input_t ParseNum(char **input_str)
{
	char *start_input = NULL;
	double num = 0.0;
	input_t input = {0, 0, 0};
	
	*input_str = SkipWhiteSpace(*input_str);
	start_input = *input_str;
	num = strtod(start_input, input_str);
	
	if (start_input < *input_str)
	{
		input.num = num;
		input.id = NUMBER;
	}
	else 
	{
		input = ParseOper(input_str);
	}
	
	return input;
}

static void ActionNum(input_t input, calc_t *calc, error_t *error)
{
	if (NUMBER == input.id)
	{
		StackPush(calc->num_stack, &(input.num));
	}
	else if (OPEN_BRACKET == input.id)
	{
		*error = LUT_handlers_g[input.id].handler_f(calc, &(input.oper));
	}
	else
	{
		*error = SYNTAX_ERROR;
	}
}

static input_t ParseOper(char **input_str)
{
	input_t input = {0,0,0};
	
	*input_str = SkipWhiteSpace(*input_str);
	
	input.id = GetOperId(**input_str);
	input.oper = **input_str;
	++(*input_str);	
	
	return input;
}

static void ActionOper(input_t input, calc_t *calc, error_t *error)
{
	*error = LUT_handlers_g[input.id].handler_f(calc, &(input.oper));
}

static void ChangeState(state_id_t *state, input_t input)
{
	switch (input.id)
	{
		case NUMBER:
			*state = WAIT_FOR_OPERATOR;
			break;
		case CLOSE_BRACKET:
			*state = WAIT_FOR_OPERATOR;
			break;
		default:
			*state = WAIT_FOR_NUM;
	}
}

static error_t HandleOper(calc_t *calc, char *oper)
{
	error_t error = SUCCESS;
	char curr_oper = 0;
	
	if (IsStackEmpty(calc->oper_stack))
	{
		StackPush(calc->oper_stack, oper);
		
		return (SUCCESS);
	}
	
	curr_oper = *(char *)StackPeek(calc->oper_stack);
	
	if (IsHigherPriority(curr_oper, *oper))
	{
		StackPush(calc->oper_stack, oper);
	}
	else /* lower/equal priority */
	{
		error = DoCalculations(calc, oper, &curr_oper);
	}
	
	return error;
}

static error_t HandleOpenParan(calc_t *calc, char *oper)
{
	StackPush(calc->oper_stack, oper);
		
	return (SUCCESS);
}

static error_t HandleCloseParan(calc_t *calc, char *oper)
{
	error_t error = SUCCESS;
	char *curr_oper = (char *)StackPeek(calc->oper_stack);
	
	while ( NULL != curr_oper && SUCCESS == error &&
			!IsStackEmpty(calc->oper_stack) &&
			!IsMatchingOpenParan(curr_oper, *oper))
	{
		double *num1 = DoOneCalc(calc, curr_oper, &error);
		ErrorAction(calc, error, num1);
		curr_oper = (char *)StackPeek(calc->oper_stack);
	}
	
	if (IsMatchingOpenParan(curr_oper, *oper))
	{
		(char *)StackPop(calc->oper_stack);
	}
	else
	{
		error = SYNTAX_ERROR;
	}
	
	return error;
}

static error_t HandlePow(calc_t *calc, char *oper)
{
	StackPush(calc->oper_stack, oper);
		
	return (SUCCESS);
}

static error_t ErrorHandler(calc_t *calc, char *oper)
{
	UNUSED(*calc);
	UNUSED(*oper);
	
	return (SYNTAX_ERROR);
}

static error_t DoCalculations(calc_t *calc, char *oper, char *curr_oper)
{
	error_t error = SUCCESS;
	
	while ( NULL != curr_oper && SUCCESS == error &&
			!IsStackEmpty(calc->oper_stack) &&
			!IsHigherPriority(*curr_oper, *oper))
	{
		double *num1 = DoOneCalc(calc, curr_oper, &error);
		StackPush(calc->num_stack, num1);
		curr_oper = (char *)StackPeek(calc->oper_stack);
	}
	
	StackPush(calc->oper_stack, oper);
	
	return error;
}

static double *DoOneCalc(calc_t *calc, char *curr_oper, error_t *error)
{
	input_id_t oper_id= GetOperId(*curr_oper);
	double *num2 = (double *)StackPop(calc->num_stack);
	double *num1 = (double *)StackPop(calc->num_stack);
	*error = LUT_opers_g[oper_id].calc_func(num1, num2);
	
	StackPop(calc->oper_stack);	
	
	return num1;
}

static int IsMatchingOpenParan(char *oper_in_stack, char oper)
{
	if (NULL == oper_in_stack)
	{
		return 0;
	}
	
	switch (oper)
	{
		case ')':
			return *oper_in_stack == '(';
		case ']':
			return *oper_in_stack == '[';
		case '}':
			return *oper_in_stack == '{';
		default:
			return 0;
	}
}

static int IsHigherPriority(char curr_oper, char oper)
{
	input_id_t oper_id1= GetOperId(oper);
	input_id_t oper_id2= GetOperId(curr_oper);
	
	return (LUT_opers_g[oper_id1].priority) > 
			(LUT_opers_g[oper_id2].priority);
}

static char *SkipWhiteSpace(char *runner)
{
	for ( ; isspace(*runner); ++runner)
	{
		/* empty loop */
	}

	return runner;
}

static error_t GeneralErrorHandler(double *num1, double *num2)
{
	UNUSED(*num1);
	UNUSED(num2);
	
	return (SYNTAX_ERROR);
}

static error_t Add(double *num1, double *num2)
{ 
	if (NULL == num1 || NULL == num2)
	{
		return (SYNTAX_ERROR);
	}
	
	*num1 += *num2;
	
	return (SUCCESS);
}

static error_t Sub(double *num1, double *num2)
{
	if (NULL == num1 || NULL == num2)
	{
		return (SYNTAX_ERROR);
	}
	
	*num1 -= *num2;
	
	return (SUCCESS);
}

static error_t Multi(double *num1, double *num2)
{
	if (NULL == num1 || NULL == num2)
	{
		return (SYNTAX_ERROR);
	}
	
	*num1 *= *num2;
	
	return (SUCCESS);
}

static error_t Div(double *num1, double *num2)
{
	if (NULL == num1 || NULL == num2)
	{
		return (SYNTAX_ERROR);
	}
	
	if (0 == *num2)
	{
		return (MATH_ERROR);
	}
	
	*num1 /= *num2;
	
	return (SUCCESS);
}

static error_t Pow(double *num1, double *num2)
{
	double result = 0.0;
	
	if (NULL == num1 || NULL == num2)
	{
		return (SYNTAX_ERROR);
	}
	
	if (*num1 < 0 && (*num2 > 0 && *num2 < 1))
	{
		return (MATH_ERROR);
	}
	
	if (*num1 == 0 && *num2 < 0)
	{
		return (MATH_ERROR);
	}
	
	result = pow(*num1, *num2);
	*num1 = result;

	return (SUCCESS);
}

static error_t OpenParan(double *num1, double *num2)
{
	if (NULL == num1 || NULL == num2)
	{
		return (SYNTAX_ERROR);
	}
	
	return (SUCCESS);
}

static error_t CloseParan(double *num1, double *num2)
{
	UNUSED(*num1);
	UNUSED(*num2);
	
	return (SUCCESS);
}

static input_id_t GetOperId(char oper)
{
	input_id_t oper_id = 0;
	
	switch (oper)
	{
	case '(':
		oper_id = OPEN_BRACKET;
		break;
	case '[':
		oper_id = OPEN_BRACKET;
		break;
	case '{':
		oper_id = OPEN_BRACKET;
		break;
	case ')':
		oper_id = CLOSE_BRACKET;
		break;
	case ']':
		oper_id = CLOSE_BRACKET;
		break;
	case '}':
		oper_id = CLOSE_BRACKET;
		break;
	case '+':
		oper_id = PLUS;
		break;
	case '-':
		oper_id = MINUS;
		break;
	case '*':
		oper_id = TIMES;
		break;
	case '/':
		oper_id = DIVISION;
		break;
	case '^':
		oper_id = POWER;
		break;
	default:
		oper_id = ILLEGAL_INPUT;
		break;
	}
	
	return oper_id;
}
