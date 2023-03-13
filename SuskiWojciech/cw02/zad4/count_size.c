
#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>
#include <time.h>
#include <inttypes.h>

#include <sys/types.h>
#include <dirent.h>

#include <sys/stat.h>
#include <ftw.h>

long long sum_bytes = 0;

int fn(const char *fpath, const struct stat *sb,
                   int typeflag) {
	if (typeflag == FTW_F) {
		struct stat statbuf;
		int status = stat(fpath, &statbuf);
		if (status == -1) {
			fprintf(stderr, "fn error");
			return 1;
		}
		printf("Nazwa: %s Rozmiar: %ld\n", fpath, statbuf.st_size);
		sum_bytes += statbuf.st_size;
	}

	return 0;
}


int main(int argc, char** argv) {
	struct dirent *pDirent;
	DIR *pDir;

	if (argc != 2) {
		fprintf(stderr, "Invalid number of args");
		return 1;
	}
	
	pDir = opendir(argv[1]);
	ftw(argv[1], fn, 10);
	


	return 0;
	/*
	if (pDir == NULL) {
		fprintf(stderr, "Cannot open directory");
		return 1;
	}
	
	struct stat statbuf;
	int status;
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
	}
	closedir(pDir);
	return 0;*/
}


