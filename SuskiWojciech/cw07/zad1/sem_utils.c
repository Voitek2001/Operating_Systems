#include "common_utils.h"


int create_sem(int id, int init) {

    int semid;
    key_t key = ftok(KEYSTRING, id);
    semid = semget(key, 1, IPC_CREAT | 0664);
    if (semid == -1) {
        perror("Creating sem");
        exit(EXIT_FAILURE);
    }
	if (semctl(semid, 0, SETVAL, init) == -1) {
		perror("Seting sem value");
		exit(EXIT_FAILURE);
	}

    return semid;
}


int open_sem(int id) {
	key_t key = ftok(KEYSTRING, id);
	if (key == -1) {
		exit(EXIT_FAILURE);
	}
	int semid = semget(key, 1, 0);
	if (semid == -1) {
		perror("Getting sem");
		exit(EXIT_FAILURE);
	}
	return semid;
}

void assign_worker(int semid) {
    struct sembuf operation = { 0, -1, 0 };
    if (semop(semid, &operation, 1) == -1) {
        perror("Decreasing sem");
    }
}

void release_worker(int semid) {
    struct sembuf operation = { 0, 1, 0 };
    if (semop(semid, &operation, 1) == -1) {
        perror("Increasing sem");
    }
}

void unlink_semaphore(int semid) {
    if (semctl(semid, 0, IPC_RMID) == -1) {
        perror("Destroying sem");
        exit(EXIT_FAILURE);
    }
}


