
#ifndef CMN_UTILS_H
#define CMN_UTILS_H

#define HOME_PATH getenv("HOME")
#define MSGMAX 4096
#define SERVER_ID 1

#include <limits.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <stdbool.h>
#include <signal.h>
#include <time.h>

char MAX_USR_EXCEED_MSG[MSGMAX] = "Osiagnieto maksymalna liczbe użytkownikow serwera!\n"
								  "Spróbuj ponownie za chwile\n"; 

typedef enum MsgType_t {
	INIT = 1,
	LIST = 2,
	TO_ALL = 3,
	TO_ONE = 4,
	STOP = 5,
	MUE = 6,
} MsgType_t;


typedef struct Client_t {
	key_t qID;
	int client_ID;
	bool active;
} Client_t;

typedef struct MsgBuff_t {
	MsgType_t mtype;
	char mtext[MSGMAX];	
	key_t qID; // key to open client queue (send to server)
	int client_ID; // server send client_id (to client) as IDfrom server to clientid 
	int sender_client_ID;
	struct tm mtime;	

} MsgBuff_t;

typedef struct MsgQueue_t {
	key_t id; // System & Posix
	char filepath[PATH_MAX];
} MsgQueue_t;


#endif
