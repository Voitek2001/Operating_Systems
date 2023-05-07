#include <mqueue.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include "common_utils.h"


bool running;

MsgQueue_t serverQueue;
MsgBuff_t msg;
int number_of_clients = 0;


void init_queue() {

	struct mq_attr attr;
	attr.mq_maxmsg = MAXCLIENT;
	attr.mq_msgsize = MSGMAX;
	
	serverQueue.id = mq_open(SQUEUE, O_RDWR | O_CREAT, 0666, &attr);
	sprintf(serverQueue.filepath, "%s", SQUEUE);
}

void get_time() {
	time_t rawtime = time(NULL);
	msg.mtime = *localtime(&rawtime);
}

void init_client() {
	

	if (number_of_clients >= MAXCLIENT) {
		strcpy(msg.mtext, "Osiągnięto maksymalna liczbe uzytkowników\n");
		msg.		
		return
	}
	number_of_clients++;
	int i;
	for (i = 0; i < MAXCLIENT; i++) {
		if (clients[i].active == false) {
			sprintf(clients[i].client_q.filepath, "%s",msg.mtext);
			msg.client_ID = i;
			break;
		}
	}
	msg.mtype = INIT;
	mqd_t cQueue = mq_open(msg.mtext, O_RDWR);
	mq_send(cQueue, &msg, sizeof(MsgBuff_t), 0);
	mq_close(cQueue);
	
}
void list_client() {

}

void to_all_client() {

}


void to_one_client() {

}

void stop_client() {

}


int main() {
	

	srand(time(NULL));	
	init_queue();
	

	while(1) {
		mq_receive(serverQueue.id, &msg.mtext, MSGMAX, NULL);
		switch (msg.mtype) {
			case INIT:
				init_client();
				break;
			case LIST:
				list_client();
				break;
			case TO_ALL:
				to_all_client();
				break;
			case TO_ONE:
				to_one_client();
				break;
			case STOP:
				stop_client();
				break;
			default:
				break;
		}
			

	}
	
	exit(EXIT_SUCCESS);
}

