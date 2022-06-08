/****************************************************************************
*Author : Gal Dagan
*Date : 
*Reviewer :
*
*Description :
*
*Lab : ol108
****************************************************************************/

#include <stdio.h>
#include "circular_buffer.h"

static void PrintDest(char *dest, int num);

int main()
{
	cbuff_t *buffer = NULL;
	char src[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g'};
	char dest[10] = {0};
	
	buffer = CBuffCreate(5);
	printf("Size after Create: %lu\n", CBuffGetSize(buffer));
	printf("IsEmpty After create: %d\n", CBuffIsEmpty(buffer));
	printf("FreeSpace after create: %lu\n", CBuffGetFreeSpace(buffer));
	
	CBuffWrite(buffer, src, 4);
	printf("\nSize after write: %lu\n", CBuffGetSize(buffer));
	printf("IsEmpty After write: %d\n", CBuffIsEmpty(buffer));
	printf("FreeSpace after write: %lu\n", CBuffGetFreeSpace(buffer));
	/*printf("Offset after write: %lu\n", CBuffGetOffset(buffer));*/
	
	CBuffRead(buffer, dest, 3);
	printf("\nSize after read: %lu\n", CBuffGetSize(buffer));
	printf("IsEmpty After read: %d\n", CBuffIsEmpty(buffer));
	printf("FreeSpace after read: %lu\n", CBuffGetFreeSpace(buffer));
	/*printf("Offset after read: %lu\n", CBuffGetOffset(buffer));*/
	PrintDest(dest, 3);
	
	CBuffWrite(buffer, src, 3);
	printf("\nSize after write: %lu\n", CBuffGetSize(buffer));
	printf("IsEmpty After write: %d\n", CBuffIsEmpty(buffer));
	printf("FreeSpace after write: %lu\n", CBuffGetFreeSpace(buffer));
	/*printf("Offset after write: %lu\n", CBuffGetOffset(buffer));*/
	
	CBuffRead(buffer, dest, 3);
	printf("\nSize after read: %lu\n", CBuffGetSize(buffer));
	printf("IsEmpty After read: %d\n", CBuffIsEmpty(buffer));
	printf("FreeSpace after read: %lu\n", CBuffGetFreeSpace(buffer));
	/*printf("Offset after read: %lu\n", CBuffGetOffset(buffer));*/
	PrintDest(dest, 3);
	
	CBuffRead(buffer, dest, 3);
	printf("\nSize after read: %lu\n", CBuffGetSize(buffer));
	printf("IsEmpty After read: %d\n", CBuffIsEmpty(buffer));
	printf("FreeSpace after read: %lu\n", CBuffGetFreeSpace(buffer));
	/*printf("Offset after read: %lu\n", CBuffGetOffset(buffer));*/
	PrintDest(dest, 1);
	
	CBuffWrite(buffer, src, 7);
	printf("\nSize after write: %lu\n", CBuffGetSize(buffer));
	printf("IsEmpty After write: %d\n", CBuffIsEmpty(buffer));
	printf("FreeSpace after write: %lu\n", CBuffGetFreeSpace(buffer));
	/*printf("Offset after write: %lu\n", CBuffGetOffset(buffer));*/
	
	CBuffRead(buffer, dest, 7);
	printf("\nSize after read: %lu\n", CBuffGetSize(buffer));
	printf("IsEmpty After read: %d\n", CBuffIsEmpty(buffer));
	printf("FreeSpace after read: %lu\n", CBuffGetFreeSpace(buffer));
	/*printf("Offset after read: %lu\n", CBuffGetOffset(buffer));*/
	PrintDest(dest, 7);
	
	CBuffDestroy(buffer);
	
	return 0;
}

static void PrintDest(char *dest, int num)
{
	int i = 0;
	
	for (;i < num; ++i)
	{
		printf("%c ", dest[i]);
	}
	
	printf("\n");
}
