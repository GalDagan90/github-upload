#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

int main()
{
	key_t key;
	int shm_id;
	char string[15] = {'\0'};
	char *shm_address;
	/* struct shmid_ds shm_ds; */

	key = ftok("my_shmem", 1);
	shm_id = shmget(key, __getpagesize(), 0666 | IPC_CREAT);
	shm_address = shmat(shm_id, NULL, 0);
	strcpy(string, shm_address);
	printf("%s\n", string);
	shmdt(shm_address);

	/* shmctl(shm_id, IPC_STAT, &shm_ds);
	if (0 == shm_ds.shm_nattch)
	{
		shmctl(shm_id, IPC_RMID, NULL);
	} */

	return (0);
}