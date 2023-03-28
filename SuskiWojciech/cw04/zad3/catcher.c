#include "catcher.h"
#include <time.h>
#include <sys/wait.h>


int request_cnt = 0;
const int SIGNAL_ID = SIGUSR1;
int inf_loop_pID = 0;


void range1_100() {
	printf("Numbers from 1 to 100\n");
	for (int i = 1; i <= 100; i++) {
		printf("%d ", i);
	}
	printf("\n");
}


void get_time() {
	time_t rawtime;
	struct tm* timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	printf("Current local time: %s", asctime(timeinfo));
	
}


void get_req_num() {
	printf("Request number: %d\n", request_cnt);
}


void loop_time() {
	int pID = fork();
	switch (pID) {
		case -1:
			fprintf(stderr, "Fork erorr");
			exit(EXIT_FAILURE);
		case 0:
			//child
			// this process is going to be killed so don't have to return anything
			while (1) {
				get_time();
				sleep(1);
			}

		default:
			//parent
			inf_loop_pID = pID;
	} 	
}




void inform_sender(int sender_pid) {
	kill(sender_pid, SIGNAL_ID);
}



void block_signal(sigset_t* change_mode_signals) {

	sigemptyset(change_mode_signals);
	sigaddset(change_mode_signals, SIGNAL_ID);

	
	// signal is now blocked, raising it will not call the signal handler
	if (sigprocmask(SIG_BLOCK, change_mode_signals, NULL) == -1) {
		fprintf(stderr, "Cannot block this SIGNAL");
	}
} 


void exit_program() {
	printf("Shuting down catcher..\n");
	exit(EXIT_SUCCESS);
}



int main() {
	
	printf("My PID is: %d\n", getpid());
	CATCHER_MOD curr_mode;
	
	
	sigset_t change_mode_signals;
	block_signal(&change_mode_signals);
	siginfo_t sig_info;
	
	
	while(1) {
		printf("Waiting for signal from sender...\n");	
		if (sigwaitinfo(&change_mode_signals, &sig_info) == -1) {
			fprintf(stderr, "Error while waiting on signal");
			exit(1);
		}

		if (inf_loop_pID != 0) {
			kill(inf_loop_pID, SIGKILL);
			inf_loop_pID = 0;
		}
		
		// consider to remove
		if (curr_mode == sig_info.si_value.sival_int) {
			//printf("Received same signal twice, nothing to do...\n");
			//continue;
		} 

		curr_mode = sig_info.si_value.sival_int;

		// inform sender that catcher received message
		inform_sender(sig_info.si_pid);
		switch(curr_mode) {
			case RANGE1_100: range1_100();	 break;
			case TIME: 	 get_time();	 break;
			case REQ_NUM:	 get_req_num();  break;
			case LOOP_TIME:  loop_time();	 break;
			case EXIT: 	 exit_program();
			
		}
		// increment counter of requests
		request_cnt++;
		
	}

}


