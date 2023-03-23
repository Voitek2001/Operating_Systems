#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

char* get_file_start(char const* filename, int nmb_char) {
	
	FILE* file = fopen(filename, "r");
	if (file == NULL) {
		exit(1);
	}
	fseek(file, 0, SEEK_END);
	int size = ftell(file);
	fseek(file, 0, SEEK_SET);
	
	if (size < nmb_char) {
		return NULL;
	}
	char* buffer = calloc(sizeof(char), nmb_char + 1);
	fread(buffer, sizeof(char), nmb_char, file);
	fclose(file);

	return buffer;
}


void browse_catalog(char* path, char* start_str) {
	
	DIR *pDir = opendir(path);
	if (pDir == NULL) {
		fprintf(stderr, "Cannot open directory %s", path);
		return;
	}
	struct dirent *pDirent;
	struct stat statbuf;
	int status;
	char curr_file[PATH_MAX];

	pid_t pID = fork();
	if (pID == 0) {
	while ((pDirent = readdir(pDir)) != NULL) {
		
		
		sprintf(curr_file, "%s/%s", path, pDirent->d_name);
		status = stat(curr_file, &statbuf);
		if (status == -1) {
			fprintf(stderr, "Failed getting info about file [%s]", pDirent->d_name);
			return ;
		}
		sprintf(curr_file, "%s/%s", path, pDirent->d_name);
		if (S_ISDIR(statbuf.st_mode)) {
			if (strcmp(pDirent->d_name, ".") != 0 && strcmp(pDirent->d_name, "..") != 0) {
			
				browse_catalog(curr_file, start_str);
			}
		}
		else if(S_ISREG(statbuf.st_mode)) {
	
			char* buffer = get_file_start(curr_file, strlen(start_str));
			if (strcmp(buffer, start_str) == 0) {
			
				printf("regular file:%s , pid:%d\n", curr_file, getpid());
			}
		}

	}
	
	exit(0);
	}

	wait(NULL);
}

int main(int argc, char** argv) {
	if (argc != 3) {
		fprintf(stderr, "Invalid number of arguments");
		return 1;
	}
	const char* dir_path = argv[1];
	char* start_str = argv[2];
	/*
	if (strncmp(dir_path, "..", 2) == 0) {
		fprintf(stderr, "invalid directory name");
		return 2;
	} 
	*/
	char path[PATH_MAX];
	sprintf(path, "%s", dir_path);
	printf("input path: %s\n", path);
	browse_catalog(path, start_str);

	return 0;
}
