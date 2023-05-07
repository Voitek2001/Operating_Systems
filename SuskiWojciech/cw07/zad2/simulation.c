#include "common_utils.h"
#include "shm.h"
#include "sem_utils.h"


char random_haircut() {
	return (char) (rand() % 100);
}

char lob[10];

int main(int argc, char** argv) {
	
	int nhdress = atoi(argv[1]);
	int nchars = atoi(argv[2]);
	int lobby_cap = atoi(argv[3]);
	int ncustom = atoi(argv[4]);
	snprintf(lob, 10, "%d", lobby_cap);
	srand(time(NULL));


	char *shared = attach_shared_memory(KEYSTRING, BUFFER_SIZE);
	shared[0] = '\0';
	//unlink_semaphore(SEM_QUEUE);
	sem_t* semid = create_sem(SEM_QUEUE, nchars);

	for (int i = 0; i < nchars; i++) {
		// increase semaphore
		// pick one hairdress for this, for sure nhdressers >= nchars
		pid_t pID = fork();
		switch(pID) {
			case -1:
				perror("Fork");
				exit(EXIT_FAILURE);
			case 0:
				execl("./hairdresser", "hairdresser", NULL);
				exit(EXIT_SUCCESS);
			default:
				break;
		}
	}


	for (int i = 0; i < ncustom; i++) {
		// decrease semaphore
		pid_t pID = fork();
		switch(pID) {
			case -1:
				perror("Fork");
				exit(EXIT_FAILURE);
			case 0:
				execl("./client", "client", lob ,NULL);
				fflush(NULL);
				exit(EXIT_SUCCESS);
			default:
				if (i >= nchars) {
					sleep(1.5);
				}
				break;
		}
	}
	while(wait(NULL) > 0);

	destroy_shared_memory(KEYSTRING);
	unlink_semaphore(SEM_QUEUE);
	exit(EXIT_SUCCESS);
}
