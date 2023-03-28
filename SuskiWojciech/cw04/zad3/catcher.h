#ifndef CATCHER_UTILS
#define CATCHER_UTILS

#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>


typedef enum {
	RANGE1_100,
	TIME,
	REQ_NUM,
	LOOP_TIME,
	EXIT
} CATCHER_MOD;


//Function print on stdout numbers from 1 to 100 
void range1_100();

//Function print on stdout current time and date
void get_time();

//Function print on stdout number of requests
void get_req_num();

//Function print on stdout current time and date while next signal appear
void loop_time();

//Function inform process "sender_pid" by raising sigusr1 there
void inform_sender(int sender_pid);

//Function exit catcher and print information about it
void exit_program();

#endif



