#include <stdio.h>
#include <string.h>
#include <stdlib.h>


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

	printf("\n%s\n", buffer);
	if (fwrite(buffer, sizeof(char), strlen(buffer), file) ) {
		fprintf(stderr, "Error(fwrite)");
		exit(1);
	}
	printf("jd");

}


char* get_output(const char* filename, const char* out_filename ,int number_of_chars) {
	
	FILE* file = open_file_to_read(filename);
	long size = find_size(file);
	char* buffer = calloc(number_of_chars + 1, sizeof(char));
	if (buffer == NULL) {
		fprintf(stderr, "Error(calloc)");	
		exit(1);
	}
	
	int number_of_read = size / number_of_chars;
	int rest = size % number_of_chars;
	printf("%d, size, %d, rest, ", number_of_read, rest);
	FILE* out_file = fopen(out_filename, "w");
	for (int i = 0; i < number_of_read; i++) {
		if(fread(buffer, sizeof(char), number_of_chars, file) == -1) {
			fprintf(stderr, "Error(fread)");
			exit(1);
		}
		printf("inside loop %s", buffer);
		write_rev(out_file, buffer);
	}
	if(fread(buffer, sizeof(char), rest, file) == -1) {
		fprintf(stderr, "Error(fread)");
		exit(1);
	}
	//printf("buf %s", buffer);
	write_rev(out_file, buffer);

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

int main(int argc, char** argv) {

	const char *in_file = argv[1];
	const char *out_file = argv[2];
	
	//read content from file
	char* buffer = get_output(in_file, out_file,1024);
	
	//reverse buffer
	//char* rev_st = reverse_buffer(buffer);
	
	//write to file
	//write_to_file(out_file, rev_st);
	
	return 0;
}
