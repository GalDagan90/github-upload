#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

int main()
{
	key_t key;
	int shm_id;
	char string[] = "Test string";
	char *shm_address;

	key = ftok("my_shmem", 1);
	shm_id = shmget(key, __getpagesize(), 0666 | IPC_CREAT);
	shm_address = shmat(shm_id, NULL, 0);
	strcpy(shm_address, string);
	shmdt(shm_address);

	return (0);
}