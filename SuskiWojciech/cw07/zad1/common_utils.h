#ifndef __COMMON_UTILS__
#define __COMMON_UTILS__

#define KEYSTRING getenv("HOME")
#define BUFFER_SIZE 4096
#define SEM_QUEUE 0

#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>

#endif
