#include <stdio.h>


int find_size(FILE* file) {

	if (fseek(file, 0, SEEK_END) != 0) {
		fprintf(stderr, "Error(fseek)");
		exit(1);
	}

	long size = ftell(file);
	if (size == -1) {
		fprintf(stderr, "Error(ftell)");
		exit(1);
	}
	if (fseek(file, 0, SEEK_CUR) != 0) {
		fprintf(stderr, "Error(fseek)");
		exit(1)
	}
	return size;

}

FILE* open_file_to_read(const char*filename) {

	FILE* file = fopen(filename, "r");
	if (file == NULL) {
		fprintf(stderr, "Error(fopen)");
		exit(1);
	}
}

void close_file(FILE* file) {
	
	if (fclose(file) != 0) {
		fprintf(stderr, "Error(fclose)");
		exit(1);	
	}
}

char* get_output(const char* filename) {
	
	FILE* file = open_file_to_read(filename);
	long size = find_size(file);
	char* buffer = calloc(size, sizeof(char));
	if (buffer == NULL) {
		fprintf(stderr, "Error(calloc)");	
		exit(1);
	}
	if(fread(buffer, sizeof(char), size, file) != size) {
		fprintf(stderr, "Error(fread)");
		exit(1);
	}
	close_file(file);
	return buffer;

} 

void write_to_file(const char* filename, char* buffer) {
	FILE* file = fopen(filename, "r");
	if (file == NULL) {
		fprintf(stderr, "Error(fopen)");
		exit(1);
	}
	if (fwrite() ) {
		fprintf(stderr, "Error(fwrite)");
		exit(1);
	}
	if (fclose(file) != 0) {
		fprintf(stderr, "Error (fclose)");
		exit(1);
	}

}

