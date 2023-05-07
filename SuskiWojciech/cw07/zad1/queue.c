
#include <string.h>

#include "queue.h"
#include "common_utils.h"


char q_pop(char *queue) {
	char elem = queue[0];
	memcpy(queue, queue+1, strlen(queue) +1);
	return elem;
}

void q_push(char *queue, char elem) {
	if(q_full(queue)) {
		fprintf(stderr, "Queue full\n");
		exit(EXIT_FAILURE);
	}
	int size = strlen(queue);
	queue[size] = elem;
	queue[size+1] = '\0';
}


bool q_full(char *queue) {
	return (strlen(queue)+1 == BUFFER_SIZE);
}

bool q_empty(char *queue) {
	return (strlen(queue) == 0);
}
