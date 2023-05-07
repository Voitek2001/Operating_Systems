#include <stdio.h>
#include <stdlib.h>
#include "common_utils.h"
#include <string.h>
#include <unistd.h>
#include <string.h>

#define MAX_CLIENTS 4

int clients_keys[MAX_CLIENTS] = { [ 0 ... MAX_CLIENTS-1 ] = -1 };

Client_t clients[MAX_CLIENTS];

void set_default(Client_t* clients) {
	for (int z = 0; z < MAX_CLIENTS; z++) {
		clients[z].active = false;
	}
}

int number_of_clients= 0;
int s_queue_id;
struct MsgBuff_t msg;

void init_client() {	
	int curr_client = msgget(msg.qID, 0);
	if (curr_client == -1) {
		perror("open client");
		exit(EXIT_FAILURE);
	}

	if (number_of_clients >= MAX_CLIENTS) {
		snprintf(msg.mtext, MSGMAX, "%s", MAX_USR_EXCEED_MSG);
		msg.mtype = MUE;
		if (msgsnd(curr_client, &msg, sizeof(MsgBuff_t), 0) == -1) {
			perror("sending INIT");
			exit(EXIT_FAILURE);
		}
		return ;
	}
	
	int i = 0;	
	for (i = 0; i < MAX_CLIENTS; i++) {
		if (clients[i].active == false) {
			clients[i].client_ID = i;
			clients[i].active = true;
			clients[i].qID = msg.qID;
			msg.client_ID = i;
			break;
		}
	}
	number_of_clients++;
	msg.mtype = INIT;
	msgsnd(curr_client, &msg, sizeof(MsgBuff_t), 0);

}

void stop_client() {
	/*
	 * required- msg.clientID
	 * queue is destroyed by client 
	 */
	clients[msg.client_ID].active = false;	
}




void to_one_client() {
	/*
	 * required - msg.text(don't need to change)
	 * msg.clientID (to who send)
	 * 
	 */
	int curr_client; 
	msg.mtype = TO_ONE;
	if (msg.client_ID > MAX_CLIENTS || clients[msg.client_ID].active == false) {
		sprintf(msg.mtext, "Problem to identify user with ID:%d", msg.client_ID);
		int curr_client = msgget(clients[msg.sender_client_ID].qID, 0);
		msgsnd(curr_client, &msg, sizeof(MsgBuff_t), 0);
		return;
	}
	curr_client = msgget(clients[msg.client_ID].qID, 0);
	if (msgsnd(curr_client, &msg, sizeof(MsgBuff_t), 0) == -1) {
		perror("sending_one_client");
		exit(EXIT_FAILURE);
	}
	

}

void to_all_clients() {
	
	/*
	 * required - msg.mtext (don't have to change it);
	 */
	msg.mtype = TO_ALL;
	for (int i = 0; i < MAX_CLIENTS; i++) {
		if (i != msg.sender_client_ID && clients[i].active == true) {
			int curr_client = msgget(clients[i].qID ,0);		
			if (msgsnd(curr_client, &msg, sizeof(MsgBuff_t), 0) == -1) {
				perror("all_clients_sending");
				exit(EXIT_FAILURE);
			}
		}
	}
}


void list_clients() {
	printf("Listing...\n");
	msg.mtype = LIST;
	for (int i = 0; i < MAX_CLIENTS; i++) {
		if (clients[i].active == true) {
			sprintf(msg.mtext + strlen(msg.mtext), "ID %d is running...\n", i);
		}
	}
	int curr_client = msgget(clients[msg.client_ID].qID, 0);
	msgsnd(curr_client, &msg, sizeof(MsgBuff_t), 0);
}


void signal_handler(int sig) {
	printf("Shuting down server\n");
	for (int i = 0; i < MAX_CLIENTS; i++) {
		if (clients[i].active == true) {
			//setup msg
			msg.mtype = STOP;
			// send to client
			int curr_client = msgget(clients[i].qID, 0);
			if (curr_client == -1) {
				printf("erro1");
			}
			int x = msgsnd(curr_client, &msg, sizeof(MsgBuff_t), 0);
			if (x == -1) {
				printf("error2");
				exit(EXIT_FAILURE);
			}
		}
	}
	// remove server queue
	struct msqid_ds q_buffer;
	if (msgctl(s_queue_id, IPC_RMID, &q_buffer) == -1) {
		perror("removing queue");
		exit(EXIT_FAILURE);
	}
}


int main(int argc, char** argv) {
	
	set_default(clients);
	// attach signal handler
	signal(SIGINT, signal_handler);

	key_t key = ftok(HOME_PATH, SERVER_ID);
	if (key == -1) {
		perror("ftok");
		exit(EXIT_FAILURE);
	}
	// flag to create a queue 
	// IPC_EXCL - return error if given queue exists

	int flag = IPC_CREAT | 0666;
	s_queue_id = msgget(key, flag);
	if (s_queue_id == -1) {
		perror("creating_queue");
		exit(EXIT_FAILURE);
	}
	
	
	while (1) {
		// wait for msg
		int size = msgrcv(s_queue_id, &msg, sizeof(MsgBuff_t), 0, 0);
		if (size == -1) {
			perror("receive_msg");
			exit(EXIT_FAILURE);
		}
		switch (msg.mtype) {
			case INIT:
					init_client();
					break;
			case LIST:
					list_clients();
					break;
			case TO_ALL:
					to_all_clients();
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


