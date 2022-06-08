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
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "hash_table.h"

#define UNUSED(x) (void)(x)
#define HASH_SIZE 11
#define RAND_PRIME 23
#define ARR_SIZE 30

int IsMatch(const void *data, const void *to_comp);
size_t HashFunc(void *data);
int Print(void *data, void *param);
int Multiply(void *data, void *param);
static void InitArr(int *arr);
static void TestHashInsert(hash_table_t *hash, int *arr);
static void TestHashRemove(hash_table_t *hash, int *arr);
static void TestHashForEach(hash_table_t *hash, int *arr);
static void TestDict(void);
size_t StringHashFunc(void *str);
int StringIsMatch(const void *data, const void *to_comp);

int main(void)
{
	hash_table_t *hash = HashTableCreate(HASH_SIZE, &IsMatch, &HashFunc);
	int rand_arr[ARR_SIZE] = {0};
	
	assert(NULL != hash);
	assert(0 == HashTableGetSize(hash));
	assert(HashTableIsEmpty(hash));
	
	InitArr(rand_arr);
	TestHashInsert(hash, rand_arr);
	TestHashRemove(hash, rand_arr);
	TestHashForEach(hash, rand_arr);
	
	HashTableDestroy(hash);
	
	puts("");
	puts("Implementation Tests Done\n");
	
	/*TestDict();*/
	
	return (0);
}

int IsMatch(const void *data, const void *to_comp)
{
	assert(NULL != data);
	assert(NULL != to_comp);
	
	return (*(int *)data == *(int *)to_comp);
}

int Print(void *data, void *param)
{
	printf("%d\n", *(int *)data);
	UNUSED(param);
	
	return (0);
}

int Multiply(void *data, void *param)
{
	UNUSED(param);
	(*(int *)data) *= 2;
	
	return (0);
}

size_t HashFunc(void *data)
{
	assert(NULL != data);
	
	return (*(size_t *)data % RAND_PRIME); 
}

static void InitArr(int *arr)
{
	int i = 0;
	
	for (; i < ARR_SIZE; ++i)
	{
		arr[i] = rand() % 1000;
	}
}

static void TestHashInsert(hash_table_t *hash, int *arr)
{
	size_t i = 0;
	size_t initial_size = 0;
	
	for (; i < ARR_SIZE; ++i)
	{
		HashTableInsert(hash, &arr[i]);
	}
	
	initial_size = HashTableGetSize(hash);

	assert(1 == HashTableInsert(hash, &arr[7])); /* 1 = DUP_KEY_ERROR */
	assert(initial_size == HashTableGetSize(hash));
	assert(1 == HashTableInsert(hash, &arr[0]));
	assert(initial_size == HashTableGetSize(hash));
	assert(1 == HashTableInsert(hash, &arr[14]));
	assert(initial_size == HashTableGetSize(hash));
	assert(1 == HashTableInsert(hash, &arr[21]));
	assert(initial_size == HashTableGetSize(hash));
}

static void TestHashRemove(hash_table_t *hash, int *arr)
{
	size_t initial_size = HashTableGetSize(hash);
	void *data = NULL;
	int not_in_table = 1001;
	
	assert(initial_size != 0);
	
	data = HashTableFind(hash, &arr[0]);
	assert(*(int *)data == arr[0]);
	HashTableRemove(hash, data);
	assert(HashTableGetSize(hash) == initial_size - 1);
	
	data = HashTableFind(hash, &arr[7]);
	assert(*(int *)data == arr[7]);
	HashTableRemove(hash, data);
	assert(HashTableGetSize(hash) == initial_size - 2);
	
	data = HashTableFind(hash, &arr[14]);
	assert(*(int *)data == arr[14]);
	HashTableRemove(hash, data);
	assert(HashTableGetSize(hash) == initial_size - 3);
	
	data = HashTableFind(hash, &arr[21]);
	assert(*(int *)data == arr[21]);
	HashTableRemove(hash, data);
	assert(HashTableGetSize(hash) == initial_size - 4);
	
	data = HashTableFind(hash, &not_in_table);
	assert(NULL == data);
	HashTableRemove(hash, &not_in_table);
	assert(HashTableGetSize(hash) == initial_size - 4);
}

static void TestHashForEach(hash_table_t *hash, int *arr)
{
	void *empty_param = NULL;
	void *data = NULL;
	int x = 344;
	
	HashTableForEach(hash, Multiply, empty_param);
	/*HashTableForEach(hash, Print, empty_param);*/
	
	data = HashTableFind(hash, &x);
	assert(*(int *)data == x);
	
	x = 54;
	data = HashTableFind(hash, &x);
	assert(*(int *)data == x);
	
	data = HashTableFind(hash, &arr[1]);
	assert(*(int *)data == arr[1]);
	
	data = HashTableFind(hash, &arr[8]);
	assert(*(int *)data == arr[8]);
	
	data = HashTableFind(hash, &arr[15]);
	assert(*(int *)data == arr[15]);
}

int StringIsMatch(const void *data, const void *to_comp)
{
	char *str_data = (char *)data;
	char *str_to_comp = (char *)to_comp;
	
	return (0 == strcmp(str_data, str_to_comp));
}

size_t StringHashFunc(void *data)
{
	char *str = (char *)data;
	size_t hash = 5381;
	
	for (; '\0' != *str; ++str)
	{
		hash = ((hash << 5) + hash) + ((int)(*str));
	}
	
	return (hash);
}

static void TestDict(void)
{
	char file_dir[] = "/usr/share/dict/american-english";
	FILE *file = NULL;
	char line_buffer[50] = {'\0'};
	char input_str[50] = {'\0'};
	hash_table_t *hash = HashTableCreate(30000, StringIsMatch, StringHashFunc);
	
	file = fopen(file_dir, "r");
	if (NULL == file)
	{
		return;
	}
	
	while (!feof(file))
	{
		fgets(line_buffer, 50, file);
		HashTableInsert(hash, line_buffer);
	}
	
	fclose(file);
	
	printf("Enter a string for spell check:\n");
	fgets(input_str, 50, stdin);
	printf("%d\n", NULL != HashTableFind(hash, input_str));
	
	HashTableDestroy(hash);
}
