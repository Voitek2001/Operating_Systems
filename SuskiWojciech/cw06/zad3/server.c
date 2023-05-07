#include "common_utils.h"
#include <stdbool.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>

char* MUE = "Max clients limit exceed\n";
char *clients_q[MAX_CLIENTS];
int number_of_users = 0;
bool running = true;
MsgBuff_t msg;
mqd_t sQueue;


mqd_t create_queue(const char* name) {

    struct mq_attr attr;
    attr.mq_maxmsg = MAX_CLIENTS;
    attr.mq_msgsize = sizeof(MsgBuff_t);
    return mq_open(name, O_RDWR | O_CREAT, 0666, &attr);
}


void stop_client(int curr_i) {
	clients_q[curr_i] = NULL;
	number_of_users--;
}

void stop_handler(int sig_no) {
	printf("Shuting down server...\n");
	for (int i = 0 ; i < MAX_CLIENTS; i++) {
		if (clients_q[i] != NULL) {
			msg.mtype = STOP;
			mqd_t otherQueue = mq_open(clients_q[i], O_RDWR);
			mq_send(otherQueue, (char *) &msg, sizeof(MsgBuff_t), 0);
            mq_close(otherQueue);
			stop_client(i);
		}
	}
	mq_unlink(SQUEUE);
	exit(EXIT_SUCCESS);

}

void init_client() {
	
	printf("text: %s\n", msg.mtext);
	mqd_t c_queue = mq_open(msg.mtext, O_WRONLY);
   	if (c_queue == -1) {
		perror("opening queue");
		exit(EXIT_FAILURE);
	}
	if (number_of_users >= MAX_CLIENTS) {
		sprintf(msg.mtext, "%s", MUE);
	} else {
		int i;
		for(i = 0; i < MAX_CLIENTS; i++) {
			if (clients_q[i] == NULL) {
				msg.client_id_1 = i;
				sprintf(clients_q[i], "%s", msg.mtext);
				break;
			}
		}
	}
	printf("jdjdjdjdj\n");	
	if (mq_send(c_queue, (char*) &msg, sizeof(MsgBuff_t), 0) == -1) {
		perror("sending error");
		exit(EXIT_FAILURE);
	}
	printf("tuuu\n");
	if (mq_close(c_queue) == -1) {
		perror("closing queue");
		exit(EXIT_FAILURE);
	}

}

void list_client() {
	strcpy(msg.mtext, "");

	for (int i = 0; i < MAX_CLIENTS; i++) {
		if (clients_q[i] != NULL) {
			sprintf(msg.mtext + strlen(msg.mtext), "Running %d\n", i);
		}
	}
	mqd_t otherQueue = mq_open(clients_q[msg.client_id_1], O_RDWR);
    mq_send(otherQueue, (char *) &msg, sizeof(MsgBuff_t), 0);
    mq_close(otherQueue);

}
void to_all() {
	for (int i = 0; i < MAX_CLIENTS; i++) {
		if (i != msg.client_id_1 && clients_q[i] != NULL) {
			mqd_t other_client_queue = mq_open(clients_q[i], O_RDWR);
            mq_send(other_client_queue, (char *) &msg, sizeof(MsgBuff_t), 0);
            mq_close(other_client_queue);
		}
	}
}
void to_one() {
	mqd_t otherQueue = mq_open(clients_q[msg.client_id_1], O_RDWR);
    mq_send(otherQueue, (char *) &msg, sizeof(MsgBuff_t), 0);
    mq_close(otherQueue);
}


int main() {
	mq_unlink(SQUEUE);
	for(int i = 0; i < MAX_CLIENTS; i++) {
		clients_q[i] = NULL;
	}
	sQueue = create_queue(SQUEUE);
	signal(SIGINT, stop_handler);

	while (running) {
		mq_receive(sQueue, (char *) &msg, sizeof(MsgBuff_t), NULL);
		printf("msg\n\n");
		switch (msg.mtype) {
			case INIT:
				printf("init:\n");
                init_client();
				break;
            case LIST:
               	list_client(); 
                break;
            case TO_ALL:
				to_all();
                break;
            case TO_ONE:
				to_one();
                break;
            case STOP:
				stop_client(msg.client_id_1);
                break;
            default:
                printf("Command not recognized!\n");
		}
	}
}

