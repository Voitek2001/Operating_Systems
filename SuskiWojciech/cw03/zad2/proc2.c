#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv) {
	
	printf("%s", argv[0]);
	
	//
	//setbuf(stdout, NULL);

	char* path = argv[1];

	fflush(stdout);
	int status = execl("/bin/ls", "/bin/ls", path, argv[2] ,NULL);
	if (status == -1) {
		
		return 1;
	}
	return 0;	

}
