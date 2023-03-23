#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "reverse.h"
#include <time.h>
#include <sys/times.h>
#include <inttypes.h>


long find_size(FILE* file) {

	if (fseek(file, 0, SEEK_END) != 0) {
		fprintf(stderr, "Error(fseek)");
		exit(1);
	}

	long size = ftell(file);
	if (size == -1) {
		fprintf(stderr, "Error(ftell)");
		exit(1);
	}
	if (fseek(file, 0, SEEK_SET) != 0) {
		fprintf(stderr, "Error(fseek)");
		exit(1);
	}
	return size;

}

FILE* open_file_to_read(const char*filename) {

	FILE* file = fopen(filename, "r");
	if (file == NULL) {
		fprintf(stderr, "Error(fopen)");
		exit(13);
	}
	return file;
}

void close_file(FILE* file) {
	
	if (fclose(file) != 0) {
		fprintf(stderr, "Error(fclose)");
		exit(1);	
	}
}



void write_rev(FILE* file, char* buffer) {

	if (fwrite(buffer, sizeof(char), strlen(buffer), file) == -1) {
		fprintf(stderr, "Error(fwrite)");
		exit(1);
	}

}


char* reverse_file(const char* filename, const char* out_filename ,int number_of_chars) {
	
	// open file in read mode
	FILE* file = open_file_to_read(filename);
	
	// open file to write
	FILE* out_file = fopen(out_filename, "w");	
	
	// get size of file
	long size = find_size(file);

	// allocate a memory for our input	
	char* buffer = calloc(number_of_chars + 1, sizeof(char));
	if (buffer == NULL) {
		fprintf(stderr, "Error(calloc)");	
		exit(1);
	}
	
	// calculate how many times we have to read from file depends on how many chars we want to read from file
	int number_of_read = size / number_of_chars;
	int rest = size % number_of_chars;


	for (int i = 1; i <= number_of_read; i++) {
	
		fseek(file, -i*number_of_chars, SEEK_END);
		
		if(fread(buffer, sizeof(char), number_of_chars, file) == -1) {
			fprintf(stderr, "Error(fread)");
			exit(1);
		}
		buffer[number_of_chars] = 0;

		// reverse read data
		buffer = reverse_buffer(buffer);
		// write them to out_file
		write_rev(out_file, buffer);
	}

    	fseek(file, 0, SEEK_SET);
	if (rest != 0) {
		// if there are more characters write them and reverse as well
		if(fread(buffer, sizeof(char), rest, file) != rest) {
			fprintf(stderr, "Error(fread)");
			exit(1);
		}
	
	
		buffer[rest] = 0;
		buffer = reverse_buffer(buffer);
	
	}	
	
	
	write_rev(out_file, buffer);
	// close both file
	close_file(out_file);
	close_file(file);

	return buffer;

} 


void write_to_file(const char* filename, char* buffer) {
	
	FILE* file = open_file_to_read(filename);
	if (file == NULL) {
		fprintf(stderr, "Error(fopen)");
		exit(10);
	}
	if (fwrite(buffer, sizeof(char), strlen(buffer), file) ) {
		fprintf(stderr, "Error(fwrite)");
		exit(1);
	}

	close_file(file);

}


char* reverse_buffer(char *buffer) {
	
	int n = strlen(buffer);
	for (int i = 0; i < n / 2; i++) {
		char tmp = buffer[i];
		buffer[i] = buffer[n-1-i];
		buffer[n-1-i] = tmp;

	}
	return buffer;
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
	
	if (argc != 4) {
		fprintf(stderr, "Wrong number of arguments");
		return 1;
	}

	struct timespec time_buff_start, time_buff_end, delta;
        struct tms st_cpu;
        struct tms en_cpu;

	

	const char *in_file = argv[1];
	const char *out_file = argv[2];
	int number_of_chars = atoi(argv[3]);
	
	clock_gettime(CLOCK_REALTIME, &time_buff_start);	
	times(&st_cpu);
	reverse_file(in_file, out_file, number_of_chars);
	times(&en_cpu);
	clock_gettime(CLOCK_REALTIME, &time_buff_end);

	sub_timespec(time_buff_start, time_buff_end, &delta);
	double accum = (double)(time_buff_end.tv_sec - time_buff_start.tv_sec)+((double)(time_buff_end.tv_nsec-time_buff_start.tv_nsec))/NS_PER_SECOND;
	
        printf("\nReal time needed: %lf",accum);
	printf("Real Time: %d.%.9ld ns, User Time %jd, System Time %jd\n",
                        (int)(delta.tv_sec), delta.tv_nsec,
                        (intmax_t)(en_cpu.tms_utime - st_cpu.tms_utime),
                        (intmax_t)(en_cpu.tms_stime - st_cpu.tms_stime));

	
	
	return 0;
}
