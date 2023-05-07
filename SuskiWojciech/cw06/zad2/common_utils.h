#include <mqueue.h>
#include <time.h>
#include <limits.h>

#define MSGMAX 4096
#define MAXCLIENT 4
#define SQUEUE "/squeue"


typedef enum MsgType_t {
    INIT = 1,
    LIST = 2,
    TO_ALL = 3,
    TO_ONE = 4,
    STOP = 5,
    MUE = 6,
} MsgType_t;


typedef struct MsgBuff_t {
    MsgType_t mtype;
    char mtext[MSGMAX];
    key_t qID; 
    int client_ID; 
    int sender_client_ID;
    struct tm mtime;

} MsgBuff_t;

typedef struct MsgQueue_t {
    mqd_t id; 
    char filepath[PATH_MAX];
} MsgQueue_t;

typedef struct Client_t {
    MsgQueue_t client_q;
	int client_ID;
    bool active;
} Client_t;

char random_char() {
	return rand() % ('Z' - 'A' + 1) + 'A';
}
