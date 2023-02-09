// Assignment 5(A) client.c (Prog2)
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct my_msgbuf {
	long mtype;
	char mtext[200];
};

int main(void) {
	struct my_msgbuf buf;
	int msqid;
	key_t key;
	if ((key = ftok("assign5a.cpp", 'D')) == -1) { /* same key as kirk.c */
		perror("ftok");
		exit(1);
	}

	if ((msqid = msgget(key, 0644)) == -1) { /* connect to the queue */
		perror("msgget");
		exit(1);
	}

	printf("CLIENT: Ready to receive messages...\n");

	/*
		int msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp, int msgflg);
		The behavior of msgrcv() is modified by choosing a msgtyp that is positive, negative, or zero.
		Zero:	Retrieve the next message on the queue, regardless of its mtype.
		Positive:	Get the next message with an mtype equal to the specified msgtyp.
		Negative:	Retrieve the first message on the queue whose mtype field is less than or equal to the absolute value of the msgtyp argument.
	*/

	for(;;) { 	// client never quits!
		if (msgrcv(msqid, &buf, sizeof(buf.mtext), 0, 0) == -1) {
			perror("msgrcv");
			exit(1);
		}

		printf("CLIENT: \"%s\"\n", buf.mtext);
	}

	return 0;
}
