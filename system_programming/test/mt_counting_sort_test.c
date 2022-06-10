/****************************************************************************
*Author : Gal Dagan
*Date : 30.10.21	
*Reviewer :	Ido Sapan
*
*Description : Multi-threading counting sort.
*
*Lab : RD5678
****************************************************************************/
#include <stdlib.h>	/* malloc(), free() */
#include <assert.h>	/* assert()	*/
#include <stdio.h>	/* fgetc(), fopen(), fclose() */

#include "mt_counting_sort.h"

#define UNUSED(x) (void)(x)
#define NUM_CHARS 100000

static void ParseDict();

int main(void)
{
	ParseDict();

	return 0;
}

static void ParseDict()
{
	FILE *fp = NULL;
	char ch = 0;
	char *arr = NULL;
	size_t runner = 0;

	arr = (char *)malloc(NUM_CHARS);
	if (NULL == arr)
	{
		printf("Error\n");
		return;
	}

	fp = fopen("/usr/share/dict/american-english", "r");
	if (NULL == fp)
	{
		printf("Error\n");
		return;
	}

	while(runner < NUM_CHARS)
	{
		ch = fgetc(fp);
		if(feof(fp)) 
		{
			break;
		}

		if (ch != '\n')
		{
			arr[runner] = ch;
		}
		else
		{
			continue;
		}
		
		++runner;
	}

	fclose(fp);

	MTCountingSort(arr, NUM_CHARS, 14);

	for (runner = 1; runner < NUM_CHARS; ++runner)
	{
		assert((unsigned char)arr[runner] >= (unsigned char)arr[runner - 1]);
	}

	printf("\nDictionary Test Done\n\n");
	free(arr);

	return;
}

