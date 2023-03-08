#ifndef BLOCK_MANAGEMENT_H
#define BLOCK_MANAGEMENT_H
#include <stdbool.h>


typedef struct ArrayOfBlocks {
	int capacity;
	int amount;
	char** ptr;
	bool* deleted;
} ArrayOfBlocks;


ArrayOfBlocks* (*create_empty_struct)(int); 
int (*count_lines_and_words)(ArrayOfBlocks * , const char *);
char* (*show_at)(ArrayOfBlocks * , int);
void (*remove_at)(ArrayOfBlocks *  ,int);
void (*destroy)(ArrayOfBlocks *);

#endif

