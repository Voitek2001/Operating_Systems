#ifndef __SEM__
#define __SEM__
#include "common_utils.h"

sem_t* create_sem(const char* filename, int init);
void assign_worker(sem_t* sem);
void release_worker(sem_t* sem);
sem_t* open_sem(const char* filename);
void unlink_semaphore(const char* filename);
#endif
