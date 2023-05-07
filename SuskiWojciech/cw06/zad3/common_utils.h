#ifndef __COMMON_UTILS__
#define __COMMON_UTILS__

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <signal.h>
#include <mqueue.h>
#include <stdbool.h>

#define NAMELEN 8
#define SQUEUE "/QSERVER"
#define MAX_CLIENTS 4
#define MSGMAX 4096

typedef enum MsgType {
    INIT = 1,
    LIST = 2,
    TO_ONE = 3,
    TO_ALL = 4,
    STOP = 5
} MsgType;

typedef struct MsgBuff_t {
	int client_id_1;
	int client_id_2;
	MsgType mtype;
	char mtext[MSGMAX];
	struct tm mtime;
} MsgBuff_t;

char randomC() {
    return rand() % ('Z' - 'A' + 1) + 'A';
}

#endif
