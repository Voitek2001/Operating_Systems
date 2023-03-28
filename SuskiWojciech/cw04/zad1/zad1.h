

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

const int SIGNAL_ID = SIGUSR1;

typedef enum {
	IGNORE = 1,
	HANDLER = 2,
	MASK = 3,
	PENDING = 4,
	UNKNOWN = 5,
} SIG;

