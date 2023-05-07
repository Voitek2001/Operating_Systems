#include "common_utils.h"
#include "shm.h"
#include "sem_utils.h"
#include <string.h>
#include "queue.h"
#define TIMEOUT 0.1


int main() {
	
	srand(time(NULL) + getpid());
	char *queue = attach_shared_memory(KEYSTRING, BUFFER_SIZE);
	int semid = open_sem(SEM_QUEUE);
	while(1) {	
		char haircut = q_pop(queue);
		printf("Processing haircut %d...\n", haircut);
		
		sleep(2);
		if (q_empty(queue)) {
			printf("Nothing left to do. Leaving...\n");
			break;
		}
		release_worker(semid);
		sleep(TIMEOUT);
	}
	
	detach_shared_memory(queue);	

	exit(EXIT_SUCCESS);

}
