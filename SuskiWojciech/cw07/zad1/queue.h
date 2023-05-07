
#ifndef __QUEUE__
#define __QUEUE__

#include <stdbool.h>

char q_pop(char *queue);
void q_push(char *queue, char elem);
bool q_empty(char *queue);
bool q_full(char *queue);

#endif
