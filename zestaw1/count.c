#include <stdio.h>
#include "count.h"
#include <stdlib.h>



ArrayOfBlocks * create_empty_array(int size) {
	ArrayOfBlocks * new_struct = calloc(1, sizeof(ArrayOfBlocks));
	new_struct->capacity = size;
	new_struct->amount = 0;
	new_struct->ptr = calloc(sizeof(int), size);
	return new_struct;
}


int show_at(ArrayOfBlocks * curr_struct, int index) {
	int* ptr = curr_struct->ptr;
	for (int i = 0; i < curr_struct->capacity; ++i) {
		ptr[i] = i + 1;
	}
	for (int i = 0; i < curr_struct->capacity; ++i) {
		printf("%d", ptr[i]);
	}	
}

