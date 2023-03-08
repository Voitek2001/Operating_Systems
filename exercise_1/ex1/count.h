#ifndef BLOCK_MANAGEMENT_H
#define BLOCK_MANAGEMENT_H
#include <stdbool.h>


typedef struct ArrayOfBlocks {
	int capacity;
	int amount;
	char** ptr;
	bool* deleted;
} ArrayOfBlocks;


ArrayOfBlocks* create_empty_struct(int size); 
int count_lines_and_words(ArrayOfBlocks * curr_struct, const char *file_name);
char* show_at(ArrayOfBlocks * curr_struct, int index);
void remove_at(ArrayOfBlocks * curr_struct ,int index);
void destroy();

#endif

