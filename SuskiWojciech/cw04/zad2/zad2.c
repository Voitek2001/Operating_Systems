#include <stdio.h>
#include <signal.h>
#include "zad2.h"
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>


void action_handler(int sig_no, siginfo_t* info, void *context) {
	printf("[HANDLER]Signal: %d\n", sig_no);
}


void nocldstop_handler(int sig_no) {

	printf("Received signal that child process stopped\n");
	exit(0);
}

void test_nocldstop(int activate_sigact) {

	signal(SIGCHLD, nocldstop_handler);	
	
	if (activate_sigact) {
		create_sigaction(SIGCHLD, SA_NOCLDSTOP, action_handler);
	}
	pid_t pID = fork();
	int status = 0;
	switch(pID) {
		case -1:
			fprintf(stderr, "Error");
			exit(EXIT_FAILURE);
		case 0:
			//child
			printf("Raising SIGSTOP in child...\n");
			raise(SIGSTOP);
			exit(EXIT_SUCCESS);
		
		default:
			//parent
			while (wait(&status) > 0);
			exit(EXIT_SUCCESS);
	}


}

void sigusr1_handler(int sig_no, siginfo_t* sig_info, void* context) {
	printf("Signal number being delivered: %d\n", sig_info->si_signo);
	printf("Process ID of sending process: %d\n", sig_info->si_pid);
	printf("Signal value: %d\n\n", sig_info->si_value.sival_int);
}


void sigusr2_handler(int sig_no, siginfo_t* sig_info, void* context) {
	printf("Signal number being delivered: %d\n", sig_info->si_signo);
	printf("Process ID of sending process: %d\n", sig_info->si_pid);
	printf("Signal code: %d\n", sig_info->si_code);
	printf("Real user ID of sending process: %d\n", sig_info->si_uid);
	printf("Exit value or signal for process termination: %d\n", sig_info->si_status);
	printf("User time consumed:%ld \n", sig_info->si_utime);
	printf("System time consumed:%ld \n\n", sig_info->si_stime);
}


void sigchld_handler(int sig_no, siginfo_t* sig_info, void* context) {
	printf("Signal number being delivered: %d\n", sig_info->si_signo);
	printf("Process ID of sending process(child this case): %d\n", sig_info->si_pid);
	printf("Exit value or signal for process termination of child: %d\n\n", sig_info->si_status);
}


void test_siginfo() {
	
	create_sigaction(SIGUSR1, SA_SIGINFO, sigusr1_handler);	
	create_sigaction(SIGUSR2, SA_SIGINFO, sigusr2_handler);
	create_sigaction(SIGCHLD, SA_SIGINFO, sigchld_handler);
	
	//raise(SIGUSR1);
	sigval_t signal_value = {9};
	sigqueue(getpid(), SIGUSR1, signal_value);

	raise(SIGUSR2);
	pid_t pID = fork();
	int status = 0;
	switch(pID) {
		case -1:
			fprintf(stderr, "Error");
			exit(EXIT_FAILURE);
		case 0:
			//child
			exit(EXIT_FAILURE);
		
		default:
			//parent
			exit(EXIT_SUCCESS);
	}

}


void test_resethand() {

	create_sigaction(SIGUSR1, SA_RESETHAND, action_handler);
	raise(SIGUSR1);
	raise(SIGUSR1);	
} 

void create_sigaction(int signal_to_block, int flags, void(*action)(int, siginfo_t*, void*)) {

	struct sigaction act;
	act.sa_sigaction = action;
	act.sa_flags = flags;
	sigemptyset(&act.sa_mask);
	
	if (sigaction(signal_to_block, &act, NULL) == -1) {
		fprintf(stderr, "Failed setting sigaction SIGCHLD");
		exit(1);
	}
}



int main(int argc, char** argv) {
	
	
	//test_siginfo();
	//test_nocldstop(0);
	test_resethand();
	return 0;
}


