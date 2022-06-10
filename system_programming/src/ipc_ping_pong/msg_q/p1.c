#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct msg_buffer
{
	long mtype;
	char mtext[10];
}message;


int main()
{
	key_t key;
	int msg_id;

	char text1[] = "Gal";
	char text2[] = "Test";
	char text3[] = "Message";

	key = ftok("my_msg_q", 1);
	msg_id = msgget(key, 0666 | IPC_CREAT);
	
	message.mtype = 1;
	
	strcpy(message.mtext, text1);
	msgsnd(msg_id, &message, sizeof(message), 0);
	printf("Sent: %s\n", message.mtext);

	strcpy(message.mtext, text2);
	msgsnd(msg_id, &message, sizeof(message), 0);
	printf("Sent: %s\n", message.mtext);

	strcpy(message.mtext, text3);
	msgsnd(msg_id, &message, sizeof(message), 0);
	printf("Sent: %s\n", message.mtext);

	return (0);
}
