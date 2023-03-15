#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>


void browse_catalog(char* path) {
	
	DIR *pDir = opendir(path);
	if (pDir == NULL) {
		fprintf(stderr, "Cannot open directory %s", path);
		return;
	}
	struct dirent *pDirent;
	struct stat statbuf;
	int status;
	char new_path[PATH_MAX];
	char curr_file[PATH_MAX];
	printf("path inf: %s\n", path);

	pid_t pID = fork();
	if (pID == 0) { 
	while ((pDirent = readdir(pDir)) != NULL) {
		
		sprintf(curr_file, "%s/%s", path, pDirent->d_name);
		status = stat(curr_file, &statbuf);
		if (status == -1) {
			fprintf(stderr, "Failed getting info about file [%s]", pDirent->d_name);
			return ;
		}
		if (S_ISDIR(statbuf.st_mode)) {
			if (strncmp(pDirent->d_name, ".", 1) == 0) {
				continue;
			} 
			
			//fork();
			sprintf(new_path, "%s/%s", path, pDirent->d_name);
			printf("tu %s\n", new_path);
			browse_catalog(new_path);
		}
		if(S_ISREG(statbuf.st_mode)) {
		
			printf("regular file: %s\n", pDirent->d_name);
		}

	}
	}

}

int main(int argc, char** argv) {
	if (argc != 3) {
		fprintf(stderr, "Invalid number of arguments");
		return 1;
	}
	const char* dir_path = argv[1];
	char* start_str = argv[2];

	char path[PATH_MAX];
	sprintf(path, "%s", dir_path);
	printf("input path: %s\n", path);
	browse_catalog(path);
	/*
	struct dirent *pDirent;
	DIR *pDir = opendir(dir_path);
	
	struct stat statbuf;
	int status;
	while ((pDirent = readdir(pDir)) != NULL) {
		status = stat(pDirent->d_name, &statbuf);
		if (status == -1) {
			fprintf(stderr, "Failed getting info about file [%s]", pDirent->d_name);
			return 1;
		}
		if (S_ISDIR(statbuf.st_mode)) {
			if (strncmp(pDirent->d_name, ".", 1) == 0) {
				continue;
			} 
			//fork()	
			
			printf("%s\n", pDirent->d_name);
			
		}


	}

	if (fork() == 0) {
		
	}

*/
	return 0;
}
