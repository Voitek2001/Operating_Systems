#include "common_utils.h"
#include <string.h>
#include "shm.h"
#include "sem_utils.h"
#include "queue.h"

char random_haircut() {
	return (char) (rand() % 100);
}


int main(int argc, char** argv) {

	srand(time(NULL) + getpid());
	fflush(stdout);	
	int queue_size = atoi(argv[1]);
	char *queue = attach_shared_memory(KEYSTRING, BUFFER_SIZE);
	int semid = open_sem(SEM_QUEUE);
	if (strlen(queue) >= queue_size) {
        printf("Queue full\n pID: %d leaving...\n", getpid());
        fflush(NULL);
        exit(EXIT_SUCCESS);
    }

	char haircut = random_haircut();
	q_push(queue, haircut);
	assign_worker(semid);
	printf("Ordering haircut %d\n", haircut);
    fflush(stdout);
	detach_shared_memory(queue);
	exit(EXIT_SUCCESS);
			
}
