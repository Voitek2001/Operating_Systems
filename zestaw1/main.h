#ifndef BLOCK_MANAGEMENT_H
#define BLOCK_MANAGEMENT_H



typedef struct {
	int capacity;
	int amount;
	int* ptr;
} ArrayOfBlocks;

ArrayOfBlocks * createEmptyArray(int sizeOfArray);

//ArrayOfBlocks create_structure(int sizeOfArray);
//int count_lines_and_words();
int show_at(ArrayOfBlocks * curr_struct, int index);
//int remove_at(int index);
//int destroy();

#endif

