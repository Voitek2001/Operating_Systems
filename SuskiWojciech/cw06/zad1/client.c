#include <stdio.h>
#include <stdlib.h>
#include "common_utils.h"
#include <unistd.h>
#include <string.h>
#include <time.h>

int c_queue_id;
key_t client_key;
int server_qID;
int client_id;
// klient wpisuje do kolejki servera
// server wpisuje do kolejki klientow
struct MsgBuff_t msg;
struct msqid_ds q_buffer;
char command[1024] = "";


void fill_msg(MsgType_t mtype, char* mtext, key_t qID, int client_ID) {
	msg.mtype = mtype;
	snprintf(msg.mtext, MSGMAX, "%s", mtext);
	msg.qID = qID;
	msg.client_ID = client_ID;
}

void init_msg() {
	//fill_msg(INIT, NULL, NULL, client_key);	
	msg.mtype = INIT;
	msg.qID = client_key;

	if (msgsnd(server_qID, &msg, sizeof(MsgBuff_t), 0) == -1) {
		perror("Sending INIT msg");
		exit(EXIT_FAILURE);
	}
	if (msgrcv(c_queue_id, &msg, sizeof(MsgBuff_t), 0, 0) == -1) {
		perror("Receiving INIT msg");
		exit(EXIT_FAILURE);
	}
	printf("\nclientid: %d\n", msg.client_ID);
	switch (msg.mtype) {
		case MUE:
				printf("%s", MAX_USR_EXCEED_MSG);
				exit(EXIT_FAILURE);
		default:
				client_id = msg.client_ID;
				break;
	}
	
}

void get_time() {
	time_t rawtime = time(NULL);
	msg.mtime = *localtime(&rawtime);
}

MsgType_t parse_input() {
	if (strncmp("LIST", command, 4) == 0) {
		msg.mtype = LIST;
		msg.client_ID = client_id;
		get_time();	
		msgsnd(server_qID, &msg, sizeof(MsgBuff_t), 0);
		msgrcv(c_queue_id, &msg, sizeof(MsgBuff_t), 0, 0);
		printf("%s", msg.mtext);
		return LIST;
	}
	if (strncmp("2ALL ", command, 5) == 0)  {
		msg.mtype = TO_ALL;
		msg.sender_client_ID = client_id;
		snprintf(msg.mtext, MSGMAX, "%s", command+5);
		if (msgsnd(server_qID, &msg, sizeof(MsgBuff_t), 0) == -1) {
			perror("sending2all");
			exit(EXIT_FAILURE);
		}
		get_time();	
		return TO_ALL;
	}
	if (strncmp("2ONE ", command, 5) == 0) {
		char * pch;
		int tmp_cnt = 0;
  		
		msg.mtype = TO_ONE;
		msg.sender_client_ID = client_id;
		
		pch = strtok (command," ");
		while (pch != NULL) {
			tmp_cnt++;
			if(tmp_cnt == 2) {
				msg.client_ID = atoi(pch);
			}
			if (tmp_cnt == 3) {
				snprintf(msg.mtext, MSGMAX, "%s", pch);	
			}
			pch = strtok(NULL, " ");
		}
		
		if (tmp_cnt != 3) {
			exit(EXIT_FAILURE);
		}
		get_time();	

		if (msgsnd(server_qID, &msg, sizeof(MsgBuff_t), 0) == -1) {
			perror("Sending msg");
			exit(EXIT_FAILURE);
		}

		return TO_ONE;
	}	
	if (strncmp("STOP", command, 4) == 0) {
		msg.mtype = STOP;
		if (msgsnd(server_qID, &msg, sizeof(MsgBuff_t), 0) == -1) {
			perror("Sending msg");
			exit(EXIT_FAILURE);
		}
		return STOP;
	}
	return -1;
}


void signal_handler(int sig) {
	msg.mtype = STOP;
	msg.client_ID = client_id;
	msgsnd(server_qID, &msg, sizeof(MsgBuff_t), 1);
	struct msqid_ds q_buffer;
	msgctl(c_queue_id, IPC_RMID, &q_buffer);
	printf("Shuting down client...\n");
	exit(EXIT_SUCCESS);
}

void stop_client() {
	/*
	 * Shut down client because server was shutted down
	 */
	printf("Shuting down client because server has terminated...\n");
	if (msgctl(c_queue_id, IPC_RMID, &q_buffer) == -1) {
		perror("Removing client queue");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}

int main() {
	
	signal(SIGINT, signal_handler);
	srand(time(NULL));
	client_key = ftok(HOME_PATH, rand() % 255 + 1);
	if (client_key == -1) {
		perror("ftok");
		exit(EXIT_FAILURE);
	}

	//create_queue
	int flag = IPC_CREAT | 0666;
	c_queue_id = msgget(client_key, flag);
	if (c_queue_id == -1) {
		perror("creating_client_queue");
		exit(EXIT_FAILURE);
	}

	//open server msgqueue
	key_t server_key = ftok(HOME_PATH, SERVER_ID);
	if (server_key == -1) {
		perror("ftok");
		exit(EXIT_FAILURE);
	}
	
	server_qID = msgget(server_key, 0);
	if (server_qID == -1) {
		perror("open server q");
		exit(EXIT_FAILURE);
	}

	// send init message to server
	//expect answer from server
	//which contain
	init_msg();
	while (1) {
		fgets(command, 20, stdin);
		//printf("%ld", strcspn(command, "\n"));
		command[strcspn(command, "\n")] = '\0';
		

		// receive all msg sender to client
		while (msgrcv(c_queue_id, &msg, sizeof(MsgBuff_t), 0, IPC_NOWAIT) >= 0) {	
			switch (msg.mtype) {
				case LIST:
				case TO_ALL:
				case TO_ONE:
					printf("%s ", msg.mtext);
					struct tm my_time = msg.mtime;
            		printf("Msg from: %d has been sent at %02d:%02d:%02d\n",
                   	msg.client_ID,
                   	my_time.tm_hour,
                   	my_time.tm_min,
                   	my_time.tm_sec);
					break;
				case STOP:
					stop_client();	
				default:
					break;

			}
		}
		
		if(strcmp(command, "") ==0) {
			printf("\n");
			continue;
		}	
		if (parse_input() == -1) {
			printf("Nieznana komenda!!\n");
			continue;
		}
		//	perror("rcv:");
		//	exit(EXIT_FAILURE);
		
		
		fflush(NULL);
	}

	exit(EXIT_SUCCESS);
}
