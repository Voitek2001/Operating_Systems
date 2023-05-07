#include "common_utils.h"


sem_t* create_sem(const char* filename, int init) {


	sem_t *s = sem_open(filename, O_CREAT | O_EXCL, 0644, init);
	if (s == SEM_FAILED) {
		perror("sem_failed");
		exit(EXIT_FAILURE);
	}
	return s;

}


sem_t* open_sem(const char* filename) {
	sem_t* s = sem_open(filename, 0);
	if (s == SEM_FAILED) {
		perror("Getting sem");
		exit(EXIT_FAILURE);
	}
	return s;
}

void close_semaphore(sem_t *sem) {
	sem_close(sem);
}

void assign_worker(sem_t * sem) {
	sem_wait(sem);
}

void release_worker(sem_t* sem) {
	sem_post(sem);
}

void unlink_semaphore(const char* filename) {
	sem_unlink(filename);
}


