#include <signal.h>
#include <stdio.h>
#include "zad1.h"


int main() {
	
	sigset_t pending_set;
	sigpending(&pending_set);
	if (sigismember(&pending_set, SIGNAL_ID)) {
		printf("After exec signal pending: %d\n", SIGNAL_ID);
	} else {
		printf("After exec signal NOT pending: %d\n", SIGNAL_ID);
	}
	//fflush(stdout);
	raise(SIGNAL_ID);

	return 0;
}


