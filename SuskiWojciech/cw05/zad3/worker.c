#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>
#include <unistd.h>


double f(double x) { return 4 / ( x * x + 1); }
const char* pipe_name = "/tmp/pipefifo";
char write_buff[PIPE_BUF];


int main(int argc, char** argv) {
	if (argc != 3) {
		fprintf(stderr, "Invalid number of arguments pass to worker");
		exit(EXIT_FAILURE);
	}
	double a = atof(argv[1]);
	double b = atof(argv[2]);
	
	double ans = (b - a) * f((a+b)/2);
	int size = sprintf(write_buff, "%lf\n", ans);
	
	int fd = open(pipe_name, O_WRONLY);
	if (write(fd, write_buff, size) == -1) {
		fprintf(stderr, "!!!!!Writting error\n");
		exit(EXIT_FAILURE);
	}
	

	//printf("%lf, %lf", a, b);	
	return 0;
} 

