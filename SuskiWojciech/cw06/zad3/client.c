#include <stdio.h>
#include "common_utils.h"
#include <time.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>


char q_name[NAMELEN];
bool running;
mqd_t c_queue;
mqd_t s_queue;
MsgBuff_t msg;
char command[MSGMAX];
int client_id;


void create_queue() {
	q_name[0] = '/';
	for(int i = 1; i < NAMELEN; i++) {
		q_name[i] = randomC();
	}

	struct mq_attr attr;
	attr.mq_maxmsg = MAX_CLIENTS;
	attr.mq_msgsize = sizeof(MsgBuff_t);
	c_queue = mq_open(q_name, O_CREAT | O_RDWR, 0666, &attr);

	printf("queue_name: %s\n", q_name);
	if (c_queue == -1) {
		perror("creating q");
		exit(EXIT_FAILURE);
	}
	s_queue = mq_open(SQUEUE, O_RDWR);
	if(s_queue == -1) {
		perror("opeing server q");
		exit(EXIT_FAILURE);
	}
}


void stop_handler() {
	msg.mtype = STOP;
	mq_send(s_queue, (char*) &msg, sizeof(MsgBuff_t), 0);
	exit(EXIT_SUCCESS);
	
}

void parse_input() {
	if (strncmp("LIST", command, 4) == 0) {
		msg.mtype = LIST;
		mq_send(s_queue, (char*) &msg, sizeof(MsgBuff_t), 0);
		mq_receive(c_queue, (char*) &msg, sizeof(MsgBuff_t), NULL);
	}
	if (strncmp("2ALL ", command, 5) == 0) {
		msg.mtype = TO_ALL;
		msg.client_id_1 = client_id;
		sprintf(msg.mtext, "%s", command+5);
		mq_send(s_queue, (char*) &msg, sizeof(MsgBuff_t), 0);
	}
	if (strncmp("2ONE ", command, 5) == 0) {
		msg.mtype = TO_ONE;
		
		char* pch;
		int tmp_cnt = 0;
		pch = strtok(command, " ");
		while(pch != NULL) {
			tmp_cnt++;
			if (tmp_cnt == 2) {
				msg.client_id_2 = atoi(pch);
			}
			if (tmp_cnt == 3) {
				sprintf(msg.mtext, "%s", pch);
			}
			pch = strtok(NULL, " ");
		}
		if (tmp_cnt != 3) {
			exit(EXIT_FAILURE);
		}
		
		msg.client_id_1 = client_id;
		mq_send(s_queue, (char*) &msg, sizeof(MsgBuff_t), 0);
	}
	if (strncmp("STOP", command, 4) == 0) {
		msg.mtype = STOP;
		mq_send(s_queue, (char*) &msg, sizeof(MsgBuff_t), 0);
	}
}


int main() {
	running = true;
	srand(time(NULL));
	signal(SIGINT, stop_handler);	
	create_queue();

	msg.mtype = INIT;
	int size = sprintf(msg.mtext, "%s", q_name);
	msg.mtext[size] = '\0';
	printf("%s", msg.mtext);
	if (mq_send(s_queue, (char *) &msg, sizeof(MsgBuff_t), 0) == -1){
		perror("sending");
		exit(EXIT_FAILURE);
	}
	mq_receive(c_queue, (char *) &msg, sizeof(MsgBuff_t), NULL);
	printf("Client ID: %d", msg.client_id_1);
	client_id = msg.client_id_1;
	while(running) {
	
		fgets(command, MSGMAX, stdin);
		command[strcspn(command, "\n")] = '\0';

		struct timespec my_time;
		clock_gettime(CLOCK_REALTIME, &my_time);
		my_time.tv_sec += 0.1;
		while (mq_timedreceive(c_queue, (char *) &msg, sizeof(MsgBuff_t), NULL, &my_time) != -1) {
			
			switch (msg.mtype) {
				case STOP:
					stop_handler();
					break;
				default:
					printf("%s\n", msg.mtext);
					break;
			}
		}
		
		if (strcmp(command, "") == 0) {
			continue;
		}
		parse_input();


	}

	exit(EXIT_SUCCESS);
}
