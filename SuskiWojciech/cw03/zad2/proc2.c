#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv) {
	
	printf("%s", argv[0]);
	
	//
	fflush(stdout);
	//setbuf(stdout, NULL);

	char* path = argv[1];
	int status = execl("/bin/ls", path, NULL);
	if (status == -1) {
		
		return 1;
	}
	return 0;	

}
