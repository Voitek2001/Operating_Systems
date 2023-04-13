#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#define BUFFER_SIZE 128

const char* USAGE = "USAGE: ./zad2 %NUMBER_OF_CHILDEN %NUMBER_OF_INTERVALS";
/*
 * we calculate our integral using formula:
 * Integral from a to b of f(x)dx = (b - a) * f((a + b) / 2)
 *
*/


int a = 0, b = 1;
double f(double x) { return 4 / (x*x + 1); }
char write_buff[BUFFER_SIZE] = "";
char read_buff[BUFFER_SIZE] = "";


double calculate_integral(int num_of_child, double int_width) {
	//double int_width = (double) 1 / num_of_int;
	int num_of_int = (int) 1 / int_width;
	/*
	 * fd[1] - write
	 * fd[0] - read
	 */
	double final_ans = 0;
	int* pipes_to_read = calloc(num_of_child, sizeof(int));
	for (int i = 0; i < num_of_int; i++) {
		double int_width2 = int_width / num_of_child;
		for (int j = 0; j < num_of_child; j++) {
//			printf("width2: %f, width:%f, num_chld: %d\n", int_width2, int_width, num_of_child);

			int fd[2];
			pipe(fd);
			pid_t pID = fork();
			switch(pID) {
				case -1:
					exit(EXIT_FAILURE);
				case 0:
					//child
					close(fd[0]);
					double ans;
					double curr_x = int_width2*j+int_width*i + int_width2*(j+1)+int_width*i;
					//if (i == num_of_int-1 && j == num_of_child-1) {
					//	curr_x = int_width2*j+int_width*i + 1;
						//printf("przedziala =%f , b= %f\n",int_width2*j+int_width*i, 1);
						//printf("tu");
					//	ans = (1 - int_width2*j+int_width*i) * f (curr_x / 2);
					//}
					//else {
						//printf("przedziala =%f , b= %f\n",int_width2*j+int_width*i, int_width2*(j+1)+int_width*i);
						ans = int_width2 * f(curr_x / 2);
					//}
					size_t size = snprintf(write_buff, BUFFER_SIZE, "%lf", ans);
				
					ssize_t status = write(fd[1], write_buff, size);
					if (status == -1) {
					//	fprintf(stderr, "Error while writing\n");
						exit(EXIT_FAILURE);
					}
					close(fd[1]);
					exit(EXIT_SUCCESS);
				default:
					//parent
					close(fd[1]);
					pipes_to_read[j] = fd[0];
					break;
			}
	
		}
		while (wait(NULL) > 0);
		for (int j = 0; j < num_of_child; j++) {
			size_t r_size = read(pipes_to_read[j], read_buff, BUFFER_SIZE);
			read_buff[r_size] = '\0';
			final_ans += strtof(read_buff, NULL);
			close(pipes_to_read[j]);
		}
		//free(pipes_to_read);
	}
	return final_ans;
}
	

enum { NS_PER_SECOND = 1000000000 };

void sub_timespec(struct timespec t1, struct timespec t2, struct timespec *td)
{
    td->tv_nsec = t2.tv_nsec - t1.tv_nsec;
    td->tv_sec  = t2.tv_sec - t1.tv_sec;
    if (td->tv_sec > 0 && td->tv_nsec < 0)
    {
        td->tv_nsec += NS_PER_SECOND;
        td->tv_sec--;
    }
    else if (td->tv_sec < 0 && td->tv_nsec > 0)
    {
        td->tv_nsec -= NS_PER_SECOND;
        td->tv_sec++;
    }
}


int main(int argc, char** argv) {

	if (argc != 3) {
		fprintf(stderr, "Invalid number of arguments\n");
		fprintf(stderr, "%s\n", USAGE);
		exit(EXIT_FAILURE);
	}
	struct timespec time_start_buff, time_end_buff, delta;
	
	clock_gettime(CLOCK_REALTIME, &time_start_buff);
	double ans = calculate_integral(atoi(argv[2]), atof(argv[1]));
	clock_gettime(CLOCK_REALTIME, &time_end_buff);
	sub_timespec(time_start_buff, time_end_buff, &delta);
	printf("Real time: %d.%.9ld ns\n", (int)(delta.tv_sec), delta.tv_nsec);

	printf("%lf\n", ans);
	return 0;
}
	
