#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#define BUFFER_SIZE 256

const char* USAGE = "USAGE: ./zad2 %NUMBER_OF_INTERVALS"
/*
 * we calculate our integral using formula:
 * Integral from a to b of f(x)dx = (b - a) * f((a + b) / 2)
 *
*/


int a = 0, b = 1;
float f(double x) { return 4 / (x*x + 1); }
char write_buff[BUFFER_SIZE] = "";
char read_buff[BUFFER_SIZE] = "";


float calculate_integral(int num_of_int) {
	float int_width = (float) 1 / num_of_int;
	int fd[2];
	pipe(fd);
	/*
	 * fd[1] - write
	 * fd[0] - read
	 */

	float final_ans = 0;
	for (int i = 0; i < num_of_int; i++) {
		pid_t pID = fork();
		switch(pID) {
			case -1:
				exit(EXIT_FAILURE);
			case 0:
				//child
				close(fd[0]);
				
				float ans = int_width * f(int_width*(2*i + 1) / 2);
				size_t size = snprintf(write_buff, BUFFER_SIZE, "%lf", ans);
				ssize_t status = write(fd[1], &write_buff, size);
				if (status == -1) {
					fprintf(stderr, "Error while writing");
					exit(EXIT_FAILURE);
				}
				exit(EXIT_SUCCESS);
			default:
				//parent
				while (wait(NULL) > 0);
				close(fd[1]);
				size_t size = read(fd[0], &read_buff, BUFFER_SIZE);
				read_buff[size] = 0;
				final_ans = strtof(read_buff, NULL);
		}
	}

	return final_ans;
}


int main(int argc, char** argv) {

	if (argc != 2) {
		fprintf(stderr, "Invalid number of arguments");
		fprintf(stderr, "%s", USAGE);
		exit(EXIT_FAILURE);
	}

	float tmp = calculate_integral(20);
	printf("%lf", tmp);


	return 0;
}
