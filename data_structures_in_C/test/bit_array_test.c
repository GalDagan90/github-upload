#include <stdio.h> /* printf() */
#include <string.h> /* memset(), strcmp(), strcpy() */
#include <limits.h> /* CHAR_BIT */
#include "bit_array.h"

void PrintTestResult(int test_result,char *test_name);
int SetAllTest(void);
int ResetAllTest(void);
int ToStringTest(void);
int SetOnTest(void);
int SetOffTest(void);
int SetBitTest(void);
int GetValTest(void);
int FlipTest(void);
int MirrorTest(void);
int RotRTest(void);
int RotLTest(void);
int CountOnTest(void);
int CountOffTest(void);

int main(void)
{

	PrintTestResult(SetAllTest(), "SetAll");
	PrintTestResult(ResetAllTest(), "ResetAll");
	PrintTestResult(ToStringTest(), "ToString");
	PrintTestResult(SetOnTest(), "SetOn");
	PrintTestResult(SetOffTest(), "SetOff");
	PrintTestResult(SetBitTest(), "SetBit");
	PrintTestResult(GetValTest(), "GetVal");
	PrintTestResult(FlipTest(), "Flip");
	PrintTestResult(MirrorTest(), "Mirror");
	PrintTestResult(RotRTest(), "RotR");
	PrintTestResult(RotLTest(), "RotL");
	PrintTestResult(CountOnTest(), "CountOn");
	PrintTestResult(CountOffTest(), "CountOff");

	return 0;
}

void PrintTestResult(int test_result,char *test_name)
{
	char result_str[20];
	
	switch (test_result)
	{
	case 0:
		strcpy(result_str, "FAILURE");
		break;
	case 1:
		strcpy(result_str, "SUCCESS");
		break;
	default:
		strcpy(result_str, "UNKNOWN RESULT");
	}
		
	printf("Result of %s: %s\n", test_name, result_str);
}

int SetAllTest(void)
{
	bit_array_t test = 15;
	bit_array_t answer = 0;

	memset(&answer, 255, sizeof (bit_array_t));
	SetAll(&test);

	return (test == answer);
}

int ResetAllTest(void)
{
	bit_array_t test = 14568765;
	bit_array_t answer = 0;

	ResetAll(&test);

	return (test == answer);
}

int ToStringTest(void)
{
	char test[65] = {'\0'};
	char answer[65] = {'\0'};

	memset(&answer, '0', 64);
	answer[53] = '1';
	answer[63] = '1';
	answer[64] = '\0';

	ToString(1025, test);

	return (0 == strcmp(test, answer));
}

int SetOnTest(void)
{
	bit_array_t test = 11;
	bit_array_t answer = 15;

	SetOn(&test, 2);

	return (test == answer);
}

int SetOffTest(void)
{
	bit_array_t test = 15;
	bit_array_t answer = 8;

	SetOff(&test, 1);
	SetOff(&test, 0);

	return (test == answer);
}

int SetBitTest(void)
{
	bit_array_t test = 64;
	bit_array_t answer = 69;

	SetBit(&test, 0, ON);
	SetBit(&test, 2, ON);
	SetBit(&test, 5, OFF);

	return (test == answer);
}

int GetValTest(void)
{
	return GetVal(15, 1) == ON;
}

int FlipTest(void)
{
	bit_array_t test = 15;
	bit_array_t answer = 31; 

	Flip(&test, 4);

	return (test == answer);
}

int MirrorTest(void)
{
	bit_array_t test = Mirror(15);
	bit_array_t answer = 15LU << (sizeof(bit_array_t) * CHAR_BIT - 4);

	return (test == answer);
}

int RotRTest(void)
{
	bit_array_t test = 64;
	bit_array_t answer = test >> 2;

	RotR(&test, 2);

	return (test == answer);
}

int RotLTest(void)
{
	bit_array_t test = 8;
	bit_array_t answer = 8 << 3;

	RotL(&test, 3);

	return (test == answer);
}

int CountOnTest(void)
{
	bit_array_t test = CountOn(15);
	bit_array_t answer = 4;

	return (test == answer);
}

int CountOffTest(void)
{
	bit_array_t test = CountOff(15);
	bit_array_t answer = 60;

	return (test == answer);
}

