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


