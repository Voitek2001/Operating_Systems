#ifndef BLOCK_MANAGEMENT_H
#define BLOCK_MANAGEMENT_H
#include <stdbool.h>


typedef struct ArrayOfBlocks {
	int capacity;
	int amount;
	char** ptr;
	bool* deleted;
} ArrayOfBlocks;

typedef struct Block {
	int capacity;
	int amount;
	char* text;
} Block;

//ArrayOfBlocks * createEmptyArray(int sizeOfArray);

ArrayOfBlocks* create_empty_struct(int size); 
//ArrayOfBlocks create_structure(int sizeOfArray);
int count_lines_and_words(ArrayOfBlocks * curr_struct, const char *file_name);
char* show_at(ArrayOfBlocks * curr_struct, int index);
void remove_at(ArrayOfBlocks * curr_struct ,int index);
void destroy();

#endif

