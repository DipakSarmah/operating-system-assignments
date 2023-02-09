// Assignment 5(A) server.c (Prog1)
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

/*
	Each message is made up of two parts, which are defined in the template structure struct msgbuf, as defined in sys/msg.h
*/

struct my_msgbuf {
	long mtype;			// mtype is used later when retrieving messages from the queue
	char mtext[200];//mtext is the data this will be added to the queue.
};

int main(void) {
	struct my_msgbuf buf;
	int msqid;
	key_t key;	// it's just a long

	/*
		key_t ftok(const char *path, int id) generates a probably-unique key from two arguments.
		'path' has to be a file that this process can read.
		'id' is usually just set to some arbitrary char
	*/
	if ((key = ftok("assign5a.cpp", 'D')) == -1) {//t ftok(const char *path, int id);
		perror("ftok");
		exit(1);
	}

	/*
		int msgget(key_t key, int msgflg); returns the message queue ID on success, or -1 on failure.
		msgflg tells msgget() what to do with queue.
		This field must be set equal to IPC_CREAT bit-wise OR'd with the permissions for this queue.
	*/
	if ((msqid = msgget(key, 0644 | IPC_CREAT)) == -1) {//msgget(key_t key, int msgflg);
		perror("msgget");
		exit(1);
	}

	printf("SERVER: Enter lines of text, ^D to quit:\n");
	buf.mtype = 1; /* we don't really care in this case */
	while(fgets(buf.mtext, sizeof buf.mtext, stdin) != NULL) {
		int len = strlen(buf.mtext);
		/* ditch newline at end, if it exists */
		if (buf.mtext[len-1] == '\n') {
			buf.mtext[len-1] = '\0';
		}

		/*
			int msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg);
			msqid is the message queue identifier returned by msgget().
			The pointer msgp is a pointer to the data we want to put on the queue. msgsz is the size in bytes of the data to add to the queue.
			msgflg allows us to set some optional flag parameters, which are ignored here.
		*/

		if (msgsnd(msqid, &buf, len+1, 0) == -1) { /* +1 for '\0' */
			perror("msgsnd");
		}
	}

	/*
		Destroying the message queue:
		int msgctl(int msqid, int cmd, struct msqid_ds *buf);
		The 'cmd' argument tells msgctl() how to behave, IPC_RMID removes the message queue.
	*/

	if (msgctl(msqid, IPC_RMID, NULL) == -1) {
		perror("msgctl");
		exit(1);
	}
	return 0;
}
