#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>


const char* USAGE = "Available options are:\n"
			"\t1 - write numbers from range 1 to 100\n"
			"\t2 - write current time\n"
			"\t3 - write number of requests\n"
			"\t4 - write current time until next signal\n"
			"\t5 - exit program";

const int SIGNAL_ID = SIGUSR1;


void block_sig(sigset_t* confirm_catch_signals) {
	
	sigemptyset(confirm_catch_signals);
	sigaddset(confirm_catch_signals, SIGNAL_ID);
	if (sigprocmask(SIG_BLOCK, confirm_catch_signals, NULL) == -1) {
		fprintf(stderr, "Cannot block SIGUSR1 signal");
		exit(1);
	}

}


int main(int argc, char** argv) {
	
	int catcher_PID = atoi(argv[1]);
	sigset_t confirm_catch_signals;
	block_sig(&confirm_catch_signals);

	int sig_no;
	for (int i = 2; i < argc; i++) {
		
		char in_val = argv[i][0];
		if (!isdigit(in_val)) {
			printf("Input not digit\n");
			printf("Sending exit request to catcher...\n");	
			sigval_t signal_value = {4};
			sigqueue(catcher_PID, SIGNAL_ID, signal_value);
			printf("%s\n", USAGE);
			exit(1);
		}
		if (atoi(argv[i]) > 5 && atoi(argv[i]) > 0) {
			printf("Invalid operation number\n");
			printf("Sending exit request to catcher...\n");	
			sigval_t signal_value = {4};
			sigqueue(catcher_PID, SIGNAL_ID, signal_value);
			printf("%s\n", USAGE);
			exit(1);
		}

		printf("Current operation: %d\n", atoi(argv[i]));
		sigval_t signal_value = {atoi(argv[i]) - 1};
		sigqueue(catcher_PID, SIGNAL_ID, signal_value);
		
		if (sigwait(&confirm_catch_signals, &sig_no) == -1) {
			fprintf(stderr, "Error while waiting on signal");
			exit(1);
		}
		printf("Received confirmation from catcher\n");
		if (atoi(argv[i]) == 5) {
			printf("Operation 5 reached\nShuting down sender...\n");
			break;
		}
		printf("Going to next operation\n");
		sleep(5);
		

	}
	return 0;
}
