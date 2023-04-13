#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <fcntl.h>
#include <string.h>


#define BUFFER_SIZE 256

const char* pipe_name = "/tmp/pipefifo";
char read_buff[PIPE_BUF];
char arg1[BUFFER_SIZE];
char arg2[BUFFER_SIZE];


int main(int argc, char** argv) {
	
	double int_width = atof(argv[1]);
	int n = atoi(argv[2]);
	int num_of_int = 1 / int_width;
	
	mkfifo(pipe_name, 0666);;
	double ans = 0;

	for (int j = 0; j < num_of_int; j++) {
		double int_width2 = int_width / n;
		for (int i = 0; i < n; i++) {
			pid_t pID = fork();
			switch (pID) {
				case -1:
						fprintf(stderr, "!!!Fork error\n");
						exit(EXIT_FAILURE);
				case 0:
						//child
						sprintf(arg1, "%lf", j*int_width + i*int_width2);
						sprintf(arg2, "%lf", j*int_width + (i+1)*int_width2);
						execl("./worker", "worker", arg1, arg2, NULL);
						exit(EXIT_SUCCESS);
			}	
		}
	
		int fd = open(pipe_name, O_RDONLY);
		int i = 0;
		while (i < n) {
			int size = read(fd, &read_buff, PIPE_BUF);
			read_buff[size] = '\0';
			
			char delim[] = "\n";
			char* split = strtok(read_buff, delim);
			for(;split;split = strtok(NULL, delim)) {
				ans += strtod(split, NULL);
				i++;		
			}
		}
		close(fd);
	}
	remove(pipe_name);
	printf("Value: %f\n", ans);

	return 0;
}
