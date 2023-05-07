#ifndef __SEM__
#define __SEM__


int create_sem(int id, int init);
void assign_worker(int semid);
void release_worker(int semid);
int open_sem(int id);
void unlink_semaphore(int semid);
#endif
