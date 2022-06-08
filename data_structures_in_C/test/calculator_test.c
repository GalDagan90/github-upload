/******************************************************************************
 * Author: Gal Dagan                                                          *
 * Reviewer: Navot Yaari                                                      *
 * Created: 04.09.2021                                                        *
 *                                                                            *
 * Description:                                                               *
 * implementation of a calculator as a finite state machine.                  *
 *                                                                            *
 * Infinity Labs OL108                                                        *
 ******************************************************************************/

#include <stdio.h> 	/* puts() 	*/
#include <math.h>	/* pow()	*/
#include <assert.h> /* assert() */
#include <float.h> 	/* DBL_MAX, DBL_MIN */

#include "calculator.h"

void TestElemntary(void);
void TestElemntaryFail(void);
void TestParentheses(void);
void TestParenthesesFail(void);
void TestBrackets(void);
void TestBracketsFail(void);
void TestPower(void);
void TestPowerFail(void);
void TestUnary(void);
void TestUnaryFail(void);
static int OK(double res, double wanted);

int main(void)
{
	TestElemntary();
	TestElemntaryFail();
	TestParentheses();
	TestParenthesesFail();
	TestBrackets();
	TestBracketsFail();
	TestPower();
	TestPowerFail();
	TestUnary();
	TestUnaryFail();
	
	puts("");
	puts("All Good!");
	puts("");

	return 0;
}

void TestElemntary(void)
{
	error_t status = SUCCESS;
	double res = 0;

	res = Calculate("40 + 2", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("42 + 0", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("41.8 + 0.2", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("4 + 0.2", &status);
	assert(SUCCESS == status);
	assert(OK(res, 4.2)); 

	res = Calculate("40 + 02", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("041.8 + 0.2", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("40.0 + 2", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("040.0 + 2", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("44 - 2.0", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("2 - 44", &status);
	assert(SUCCESS == status);
	assert(OK(res, -42));

	res = Calculate("3 * 14", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("84 * 0.5", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("84 / 2", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("4.2 / 0.1", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("20 + 20 + 2", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("5 + 8 + 17 + 1 + 9 + 2", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("60 - 20 + 2", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("2 - 20 + 60", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("2 + 60 - 20", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("60 + 2 - 20", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("60 - 22 + 2 * 2", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("60 + 2 * 2 - 22", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("2 * 2 + 60 - 22", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("60 - 22 + 8 / 2", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("60 + 8 / 2 - 22", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("8 / 2 + 60 - 22", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("21 * 4 / 2", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("21 / 2 * 4", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("60 - 10 - 8", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("840 / 10 / 2", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("42", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("4.2", &status);
	assert(SUCCESS == status);
	assert(OK(res, 4.2));

	res = Calculate("0", &status);
	assert(SUCCESS == status);
	assert(OK(res, 0));
}

void TestElemntaryFail(void)
{
	error_t status = SUCCESS;
	double res = 0;
	
	res = Calculate("", &status);
	assert(SYNTAX_ERROR == status);
	
	res = Calculate("+ 42", &status);
	assert(SYNTAX_ERROR == status);
	assert(OK(res, 0));

	res = Calculate("42 -", &status);
	assert(SYNTAX_ERROR == status);
	assert(OK(res, 0));

	res = Calculate("42+", &status);
	assert(SYNTAX_ERROR == status);
	assert(OK(res, 0));

	res = Calculate("42 + + 42", &status);
	assert(SYNTAX_ERROR == status);
	assert(OK(res, 0));

	res = Calculate("a 42", &status);
	assert(SYNTAX_ERROR == status);
	assert(OK(res, 0));

	res = Calculate("42a", &status);
	assert(SYNTAX_ERROR == status);
	assert(OK(res, 0));

	res = Calculate("4 2", &status);
	assert(SYNTAX_ERROR == status);
	assert(OK(res, 0));

	res = Calculate("a", &status);
	assert(SYNTAX_ERROR == status);
	assert(OK(res, 0));

	res = Calculate("4 / 0", &status);
	assert(MATH_ERROR == status);
	assert(OK(res, 0));

	res = Calculate("4 - 2 / 0 + 1", &status);
	assert(MATH_ERROR == status);
	assert(OK(res, 0));

/*	res = Calculate("INF", &status);*/
/*	assert(MATH_ERROR == status);*/
/*	assert(OK(res, 0));*/

/*	res = Calculate("NAN", &status);*/
/*	printf("%f %d\n", res, status);*/
/*	assert(MATH_ERROR == status);*/
/*	assert(OK(res, 0));*/
}

void TestParentheses(void)
{
	error_t status = SUCCESS;
	double res = 0;

	res = Calculate("(40 + 2)", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("(40) + 2", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("40 + (2)", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("(40) + (2)", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("(44 - 2.0)", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("(44) - 2.0", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("44 - (2.0)", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("(44) - (2.0)", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("(84 * 0.5)", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("(84) * 0.5", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("84 * (0.5)", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("(84) * (0.5)", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("4.2 / 0.1", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("(4.2) / 0.1", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("4.2 / (0.1)", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("(4.2) / (0.1)", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("((40) + (2))", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("(((((40)) + (((2))))))", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("(20 + 20) + 2", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("20 + (24 - 2)", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("12 + 3 * (8 + 2)", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("12 + (8 + 2) * 3", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("0 - 8 + (2 + ((22 - 12) * (4 + 1)) / 100) * 20", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("(42)", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("(((4.2)))", &status);
	assert(SUCCESS == status);
	assert(OK(res, 4.2));
}

void TestParenthesesFail(void)
{
	error_t status = SUCCESS;
	double res = 0;

	res = Calculate("(4 +)", &status);
	assert(SYNTAX_ERROR == status);
	assert(OK(res, 0));

	res = Calculate("(+ 4)", &status);
	assert(SYNTAX_ERROR == status);
	assert(OK(res, 0));

	res = Calculate("+ (4)", &status);
	assert(SYNTAX_ERROR == status);
	assert(OK(res, 0));

	res = Calculate("(4) +", &status);
	assert(SYNTAX_ERROR == status);
	assert(OK(res, 0));

	res = Calculate("5 + ( + 4)", &status);
	assert(SYNTAX_ERROR == status);
	assert(OK(res, 0));

	res = Calculate("(4 +) 4", &status);
	assert(SYNTAX_ERROR == status);
	assert(OK(res, 0));

	res = Calculate("5 + ( + 4) 5", &status);
	assert(SYNTAX_ERROR == status);
	assert(OK(res, 0));

	res = Calculate("(4", &status);
	assert(SYNTAX_ERROR == status);
	assert(OK(res, 0));

	res = Calculate("4)", &status);
	assert(SYNTAX_ERROR == status);
	assert(OK(res, 0));

	res = Calculate("(4))", &status);
	assert(SYNTAX_ERROR == status);
	assert(OK(res, 0));

	res = Calculate("(((4))", &status);
	assert(SYNTAX_ERROR == status);
	assert(OK(res, 0));

	res = Calculate("(4 + 4", &status);
	assert(SYNTAX_ERROR == status);
	assert(OK(res, 0));

	res = Calculate("4) + 4", &status);
	assert(SYNTAX_ERROR == status);
	assert(OK(res, 0));

	res = Calculate("4 + (4", &status);
	assert(SYNTAX_ERROR == status);
	assert(OK(res, 0));

	res = Calculate("4 + 4)", &status);
	assert(SYNTAX_ERROR == status);
	assert(OK(res, 0));

	res = Calculate(")4 + 4", &status);
	assert(SYNTAX_ERROR == status);
	assert(OK(res, 0));

	res = Calculate("4( + 4", &status);
	assert(SYNTAX_ERROR == status);
	assert(OK(res, 0));

	res = Calculate("4 + )4", &status);
	assert(SYNTAX_ERROR == status);
	assert(OK(res, 0));

	res = Calculate("4 + 4(", &status);
	assert(SYNTAX_ERROR == status);
	assert(OK(res, 0));

	res = Calculate("(5 + 3 (* 4) + 5)", &status);
	assert(SYNTAX_ERROR == status);
	assert(OK(res, 0));

	res = Calculate("(5 / (3 - 3))", &status);
	assert(MATH_ERROR == status);
	assert(OK(res, 0));

	res = Calculate("(5 / (1 * 0) + 8)", &status);
	assert(MATH_ERROR == status);
	assert(OK(res, 0));
}

void TestBrackets(void)
{
	error_t status = SUCCESS;
	double res = 0;

	res = Calculate("[40 + 2]", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("[44] - 2.0", &status);
	assert(SUCCESS == status);

	res = Calculate("84 * [0.5]", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("[4.2] / [0.1]", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("[[40] + [2]]", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("[[[[[40]] + [[[2]]]]]]", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("0 - 8 + [2 + [[22 - 12] * [4 + 1]] / 100] * 20", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("[42]", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("[[[4.2]]]", &status);
	assert(SUCCESS == status);
	assert(OK(res, 4.2));

	res = Calculate("{40 + 2}", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("{44} - 2.0", &status);
	assert(SUCCESS == status);

	res = Calculate("84 * {0.5}", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("{4.2} / {0.1}", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("{{40} + {2}}", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("{{{{{40}} + {{{2}}}}}}", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("0 - 8 + {2 + {{22 - 12} * {4 + 1}} / 100} * 20", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("{42}", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("{{{4.2}}}", &status);
	assert(SUCCESS == status);
	assert(OK(res, 4.2));

	res = Calculate("{42}", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("([40 + 2])", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("[(40 + 2)]", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("({40 + 2})", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("{(40 + 2)}", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("{[40 + 2]}", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("[{40 + 2}]", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("{[(40 + 2)]}", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("([{40 + 2}])", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("[{40} + (2)]", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("3 + {[0 - (8)] + {2 + ([(22 - 12) * [4 + {1}]] / 100)} * 20} - 3", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));
}

void TestBracketsFail(void)
{
	error_t status = SUCCESS;
	double res = 0;

	res = Calculate("[4 +] 4", &status);
	assert(SYNTAX_ERROR == status);
	assert(OK(res, 0));

	res = Calculate("5 + [ + 4] 5", &status);
	assert(SYNTAX_ERROR == status);
	assert(OK(res, 0));

	res = Calculate("[4", &status);
	assert(SYNTAX_ERROR == status);
	assert(OK(res, 0));

	res = Calculate("4]", &status);
	assert(SYNTAX_ERROR == status);
	assert(OK(res, 0));

	res = Calculate("[4]]", &status);
	assert(SYNTAX_ERROR == status);
	assert(OK(res, 0));

	res = Calculate("[[[4]]", &status);
	assert(SYNTAX_ERROR == status);
	assert(OK(res, 0));

	res = Calculate("{4 +} 4", &status);
	assert(SYNTAX_ERROR == status);
	assert(OK(res, 0));

	res = Calculate("5 + { + 4} 5", &status);
	assert(SYNTAX_ERROR == status);
	assert(OK(res, 0));

	res = Calculate("{4", &status);
	assert(SYNTAX_ERROR == status);
	assert(OK(res, 0));

	res = Calculate("4}", &status);
	assert(SYNTAX_ERROR == status);
	assert(OK(res, 0));

	res = Calculate("{4}}", &status);
	assert(SYNTAX_ERROR == status);
	assert(OK(res, 0));

	res = Calculate("{{{4}}", &status);
	assert(SYNTAX_ERROR == status);
	assert(OK(res, 0));

	res = Calculate("(3 + 4]", &status);
	assert(SYNTAX_ERROR == status);
	assert(OK(res, 0));

	res = Calculate("(3 + 4}", &status);
	assert(SYNTAX_ERROR == status);
	assert(OK(res, 0));

	res = Calculate("[3 + 4)", &status);
	assert(SYNTAX_ERROR == status);
	assert(OK(res, 0));

	res = Calculate("[3 + 4}", &status);
	assert(SYNTAX_ERROR == status);
	assert(OK(res, 0));

	res = Calculate("{3 + 4)", &status);
	assert(SYNTAX_ERROR == status);
	assert(OK(res, 0));

	res = Calculate("{3 + 4]", &status);
	assert(SYNTAX_ERROR == status);
	assert(OK(res, 0));

	res = Calculate("[{3 + 4]}", &status);
	assert(SYNTAX_ERROR == status);
	assert(OK(res, 0));

	res = Calculate("[(3] + 4)", &status);
	assert(SYNTAX_ERROR == status);
	assert(OK(res, 0));

	res = Calculate("(3 + {4)}", &status);
	assert(SYNTAX_ERROR == status);
	assert(OK(res, 0));

	res = Calculate("{[(4)]", &status);
	assert(SYNTAX_ERROR == status);
	assert(OK(res, 0));

	res = Calculate("5 / [1 * 0] + 8", &status);
	assert(MATH_ERROR == status);
	assert(OK(res, 0));

	res = Calculate("5 / {1 * 0} + 8", &status);
	assert(MATH_ERROR == status);
	assert(OK(res, 0));
}

void TestPower(void)
{
	error_t status = SUCCESS;
	double res = 0;

	res = Calculate("42 ^ 1", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("5 ^ 2", &status);
	assert(SUCCESS == status);
	assert(OK(res, 25));

	res = Calculate("1 ^ 42", &status);
	assert(SUCCESS == status);
	assert(OK(res, 1));

	res = Calculate("42 ^ 0", &status);
	assert(SUCCESS == status);
	assert(OK(res, 1));

	res = Calculate("16 ^ 0.25", &status);
	assert(SUCCESS == status);
	assert(OK(res, 2));

	res = Calculate("39 + 2 ^ 4 - 13", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("3 * 2 ^ 3 / 5 + 37.2", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("2 ^ 3 ^ 2", &status);
	assert(SUCCESS == status);
	assert(OK(res, 512));

	res = Calculate("(2 ^ 3) ^ 2", &status);
	assert(SUCCESS == status);
	assert(OK(res, 64));

	res = Calculate("(40 + 2) ^ 1", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("[40 + 2] ^ (3 - 2)", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("4 ^ (2 - 3)", &status);
	assert(SUCCESS == status);
	assert(OK(res, 0.25));

	res = Calculate("{2 - (4)} ^ 3", &status);
	assert(SUCCESS == status);
	assert(OK(res, -8));

	res = Calculate("(2 - 4) ^ 2", &status);
	assert(SUCCESS == status);
	assert(OK(res, 4));

	res = Calculate("(2 - 4) ^ (0 - 3)", &status);
	assert(SUCCESS == status);
	assert(OK(res, -0.125));

	res = Calculate("(0 - 1) ^ 0", &status);
	assert(SUCCESS == status);
	assert(OK(res, 1));
	
	res = Calculate("1 / 0 ^ 0", &status);
	assert(SUCCESS == status);
	
	res = Calculate("101 ^ 101 ^ 101", &status);
	assert(SUCCESS == status);
}

void TestPowerFail(void)
{
	error_t status = SUCCESS;
	double res = 0;
	
	res = Calculate("-2 ^ 0.5", &status);
	assert(MATH_ERROR == status);
	assert(OK(res, 0));
	
	res = Calculate("1 / 0 ^ -2", &status);
	assert(MATH_ERROR == status);
	
	res = Calculate("2 ^", &status);
	assert(SYNTAX_ERROR == status);
	assert(OK(res, 0));

	res = Calculate("^ 2", &status);
	assert(SYNTAX_ERROR == status);
	assert(OK(res, 0));

	res = Calculate("2 ^ ^ 3", &status);
	assert(SYNTAX_ERROR == status);
	assert(OK(res, 0));

	res = Calculate("2 ^ + ^ 3", &status);
	assert(SYNTAX_ERROR == status);
	assert(OK(res, 0));

	res = Calculate("2 (^ 3)", &status);
	assert(SYNTAX_ERROR == status);
	assert(OK(res, 0));

	res = Calculate("{2 ^} 3", &status);
	assert(SYNTAX_ERROR == status);
	assert(OK(res, 0));

	res = Calculate("0 ^ (0 - 1)", &status);
	assert(MATH_ERROR == status);
	assert(OK(res, 0));

	res = Calculate("0 ^ (0 - 5)", &status);
	assert(MATH_ERROR == status);
	assert(OK(res, 0));
}

void TestUnary(void)
{
	error_t status = SUCCESS;
	double res = 0;

	res = Calculate("+40 + 2", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("40 + +2", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("44 - +2", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("+40 - -2", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("44 + -2", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("+42", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("-42", &status);
	assert(SUCCESS == status);
	assert(OK(res, -42));

	res = Calculate("+0", &status);
	assert(SUCCESS == status);
	assert(OK(res, 0));

	res = Calculate("-0", &status);
	assert(SUCCESS == status);
	assert(OK(res, 0));

	res = Calculate("40+2", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("44-2", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("40 +2", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));

	res = Calculate("44 -2", &status);
	assert(SUCCESS == status);
	assert(OK(res, 42));
}

void TestUnaryFail(void)
{
	error_t status = SUCCESS;
	double res = 0;

	res = Calculate("--42", &status);
	assert(SYNTAX_ERROR == status);
	assert(OK(res, 0));

	res = Calculate("+-42", &status);
	assert(SYNTAX_ERROR == status);
	assert(OK(res, 0));

	res = Calculate("++42", &status);
	assert(SYNTAX_ERROR == status);
	assert(OK(res, 0));
}

static int OK(double res, double wanted)
{
	double precision = 0.0000000001;

	return ((res < wanted + precision) && (res > wanted - precision));
}
