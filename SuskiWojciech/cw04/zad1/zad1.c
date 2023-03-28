#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include "zad1.h"


const char* USAGE = "Available options are: ignore, handler, mask, pending\n";


void check_which_signal(const char* cmd_str, SIG* curr_cmd) {
	if (strcmp(cmd_str, "ignore") == 0) {
		*curr_cmd = IGNORE;
	} else if (strcmp(cmd_str, "handler") == 0) {
		*curr_cmd = HANDLER;
	} else if (strcmp(cmd_str, "mask") == 0) {
		*curr_cmd = MASK;
	} else if (strcmp(cmd_str, "pending") == 0) {
		*curr_cmd = PENDING;
	} else {
		*curr_cmd = UNKNOWN;
	}
}


void signal_handler(int sigtype) {
	printf("Hello from handler, Signal: %d\n", sigtype);
}


void check_signal_pending() {
	sigset_t pending_set;
	sigpending(&pending_set);

	if (sigismember(&pending_set, SIGNAL_ID)) {
		printf("Signal pending %d \n", SIGNAL_ID);
	} else {
		printf("Signal not peding %d \n", SIGNAL_ID);
	}
}




int main(int argc, char** argv) {
	
	if (argc != 2) {
		fprintf(stderr, "Invalid number of arguments\n");
		fprintf(stderr, "%s", USAGE);
		return 1;
	}

	SIG curr_cmd;
	check_which_signal(argv[1], &curr_cmd);	

	sigset_t blockmask;
	switch(curr_cmd) {
		case IGNORE:
			signal(SIGNAL_ID, SIG_IGN);
			break;
		case HANDLER:
			signal(SIGNAL_ID, signal_handler);
			break;
		case MASK:
			sigemptyset(&blockmask);
			sigaddset(&blockmask, SIGNAL_ID);
			if (sigprocmask(SIG_BLOCK, &blockmask, NULL) == -1) {
				fprintf(stderr, "Nie udało sie zablokowac sygnału\n");
				return 1;
			}
		case PENDING:
			break;
		default:
			fprintf(stderr, "Unknown arg was given\n");
			return 1;
	}

	printf("Raising SIGUSR1 in parent...\n");
	raise(SIGNAL_ID);
	check_signal_pending();
	int childpid = fork();
	switch (childpid) {
		case -1:
			perror("Error while fork");
			exit(EXIT_FAILURE);
		case 0:
			// Child process.
			
			printf("Before raising check if pending");
			check_signal_pending();	
			//
			printf("Raising SIGUSR1 in child...\n");
			raise(SIGNAL_ID);
			printf("Check if pending in child process: ");
			check_signal_pending();
			
			printf("Running using exec\n");

			if (execl("./zad1exec", "./zad1exec", NULL) == -1) {
				fprintf(stderr, "Problem to run zad1exec");
				exit(EXIT_FAILURE);
			}

			exit(EXIT_SUCCESS);

		default:
			// parent process
			exit(EXIT_SUCCESS);

	}
	
	wait(NULL);
	return 0;
}

