
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <unistd.h>
#include <string.h>
#include "replace.h"


#define find_size(file) _Generic(file, FILE*: find_size_lib, int: find_size_sys)(file)

#ifdef SYS
#define get_output(filename) get_output_sys(filename)
#define write_to_file(buffer, filename) write_to_file_sys(buffer, filename)
#endif

#ifdef LIB
#define get_output(filename) get_output_lib(filename)
#define write_to_file(buffer, filename) write_to_file_lib(buffer, filename)
#endif


int find_size_lib(FILE* file) {
	fseek(file, 0, SEEK_END);
	int size = ftell(file);
	fseek(file, 0, SEEK_SET);
	return size;
}


int find_size_sys(int fd) {
	
	int size = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);
	return size;
}


char* get_output_lib(const char *filename) {	
	char* buff;	
	int size;
	FILE* file = fopen(filename, "r");
	size = find_size(file);
	buff = calloc(size, sizeof(char));
	if (buff == NULL) {
		fprintf(stderr, "Allocation failed\n");
		fclose(file);
	}
	fread(buff, sizeof(char), size, file);
	fclose(file);
	return buff;
}


char* get_output_sys(const char *filename) {
	char* buff;	
	int size;
	
	int fd = open(filename, O_RDONLY);
	if (fd == -1) {
		fprintf(stderr, "Error while open\n");
		return "";
	}
	size = find_size(fd);
	buff = calloc(size, sizeof(char));
	if (buff == NULL) {
		fprintf(stderr, "Allocation failed\n");
	}
	read(fd, buff, size);
	close(fd);
	return buff;	
}


void write_to_file_sys(char* buffer, const char* filename) {
	
	int fd = open(filename, O_WRONLY | O_CREAT);
	if (fd == -1) {
		fprintf(stderr, "Error while open\n");
		exit(1);
	}
	
	if (write(fd, buffer, strlen(buffer)) == -1) {
		fprintf(stderr, "Error while writting to file\n");
		exit(1);
	}
	if (close(fd) == -1) {
		fprintf(stderr, "Error while closing\n");
		exit(1);
	}
}


void write_to_file_lib(char* buffer, const char* filename) {
	FILE* file = fopen(filename, "w");
	if (fwrite(buffer, sizeof(char), strlen(buffer), file) != strlen(buffer)) {
		fprintf(stderr, "Error while writing to file\n");
		exit(1);
	}	

	if (fclose(file) != 0) {
		fprintf(stderr, "Error while closing the file\n");
		exit(1);
	}
		

}


void replace_char(char* read_file_buffer, const char to_replace, const char new_char) {
	while (*read_file_buffer) {
		if (*read_file_buffer == to_replace) {
			*read_file_buffer = new_char;
		}
		read_file_buffer++;
	}
}


int main(int argc, char** argv) {
	
	
	if (argc < 5) {
		fprintf(stderr, "Incorrect number of input arguments\n");
		return 1;
	}
	

	int char_to_replace = argv[1][0];
	int new_char = argv[2][0];	
	const char *path = argv[3];
	const char *new_path = argv[4];

	
	char* file_content = get_output(path);
	replace_char(file_content, char_to_replace, new_char);
		
	write_to_file(file_content, new_path);
	
	return 0;

}

