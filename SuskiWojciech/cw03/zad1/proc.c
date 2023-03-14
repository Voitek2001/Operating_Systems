#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


int main(int argc, char** argv) {
	
	int num_of_process = atoi(argv[1]);	
	
	pid_t pid;
	
	for (int i = 0; i < num_of_process; i++) {
		
		if (fork() == 0) {
			printf("[son] pid %d from %d\n", getpid(), getppid());
			exit(0);
		}
	}

	wait(NULL);
	printf("parent %d", getpid());
	
	
	return 0;
}


