#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>



int main() {

float input;

    int msqid;

    char pathname[] = "1.c";

    key_t key;

    int len, maxlen;

    struct clientmsgbuf {
        long mtype;
        struct {
            pid_t pid;
            float message;
        } info;
    } clientbuf;

    struct servermsgbuf {
        long mtype;
        struct {
            float message;
        } info;
    } serverbuf;

    if ((key = ftok(pathname, 0)) < 0) {
        printf("Can't generate key\n");
        exit(-1);
    }

    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
        printf("Can't get msqid\n");
        exit(-1);
    }

    clientbuf.mtype = 1;
    clientbuf.info.pid = getpid();
printf("Enter number: ");
	scanf ("%f", &input);

    clientbuf.info.message = input;

    printf("Client sends message %f\n",  clientbuf.info.message);
    if (msgsnd(msqid, (struct clientmsgbuf *) &clientbuf, sizeof(clientbuf.info), 0) < 0) {
        printf("Can't send message to queue\n");
        msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
        exit(-1);
    }

    printf("Waiting response...\n");

    if (len = msgrcv(msqid, &serverbuf, sizeof(serverbuf.info), getpid(), 0) < 0) {
        printf("Can't receive message from queue\n");
        exit(-1);
    }
    printf("Response : %f\n", serverbuf.info.message);

    return 0;

}
