#include <stdio.h>
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
	struct msqid_ds msg_q_ds;

	key = ftok("my_msg_q", 1);
	msg_id = msgget(key, 0666 | IPC_CREAT);

	msgrcv(msg_id, &message, sizeof(message), 1, 0);
	printf("Recieved: %s\n", message.mtext);

	msgctl(msg_id, IPC_STAT, &msg_q_ds);
	if (msg_q_ds.msg_qnum == 0)
	{
		msgctl(msg_id, IPC_RMID, NULL);
	}

	return (0);
}