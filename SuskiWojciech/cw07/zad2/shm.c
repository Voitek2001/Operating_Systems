#include "common_utils.h"
#include <errno.h>
#include <sys/mman.h>

int get_shared_mem(const char* filename, int size) {
	
	int desc = shm_open(filename, O_CREAT | O_RDWR, 0644);
	if (desc == -1) {
		perror("creating shm");
		exit(EXIT_FAILURE);
	}
	if (ftruncate(desc, size) == -1) {
		perror("ftruncate error");
		exit(EXIT_FAILURE);
	}
	return desc;
}

char* attach_shared_memory(const char* filename, int size) {
	int shared_memory_id = get_shared_mem(filename, size);
	if (shared_memory_id == -1) {
		exit(EXIT_FAILURE);
	}
	char *shared_memory = (char *) mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, shared_memory_id, 0);
	return shared_memory;
}

void detach_shared_memory(char* shared_memory) {
	shmdt(shared_memory);
}

void destroy_shared_memory(const char* filename) {
	shm_unlink(filename);
}

