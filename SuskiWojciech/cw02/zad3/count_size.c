
#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>
#include <time.h>
#include <inttypes.h>

#include <sys/types.h>
#include <dirent.h>

#include <sys/stat.h>

int main(int argc, char** argv) {
	struct dirent *pDirent;
	DIR *pDir;
	
	pDir = opendir(".");
	if (pDir == NULL) {
		fprintf(stderr, "Cannot open directory");
		return 1;
	}
	
	struct stat statbuf;
	int status;
	long long sum_bytes = 0;
	while ((pDirent = readdir(pDir)) != NULL) {	

		status = stat(pDirent->d_name, &statbuf);
		if (status == -1) {
			fprintf(stderr, "Failed getting info about file [%s]", pDirent->d_name);
		}
		if (S_ISDIR(statbuf.st_mode)) {
			continue;
		}
		
		printf("Nazwa: [%s] ", pDirent->d_name);
		printf("Rozmiar: %ld \n", statbuf.st_size);
		sum_bytes += statbuf.st_size;
	}
	printf("Sumaryczny rozmiar: %lld\n", sum_bytes);
	closedir(pDir);
	return 0;
}


