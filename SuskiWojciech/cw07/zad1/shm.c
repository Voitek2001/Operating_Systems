#include "common_utils.h"


int get_shared_mem(const char* filename, int size) {
	key_t key = ftok(filename, 1);
	if (key == -1) {
		perror("ftok");
		exit(EXIT_FAILURE);
	}
	return shmget(key, size, 0644 | IPC_CREAT);
}

char* attach_shared_memory(const char* filename, int size) {
	
	int shm_id = get_shared_mem(filename, size);
	if (shm_id == -1) {
		perror("shm_id");
		exit(EXIT_FAILURE);
	}
	char* shared_memory = shmat(shm_id, NULL, 0);
	if (shared_memory == (char *)(-1)) {
		perror("shmat");
		exit(EXIT_FAILURE);
	}
	return shared_memory;
}

void detach_shared_memory(char* shared_memory) {
	shmdt(shared_memory);
}

void destroy_shared_memory(const char* filename) {
	int shared_memory_id = get_shared_mem(filename, 0);
	shmctl(shared_memory_id, IPC_RMID, NULL);
}

