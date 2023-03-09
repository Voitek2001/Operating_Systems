#include <stdio.h>
#include "count.h"
#include <stdlib.h>
#include <string.h>


char command[64] = "";


ArrayOfBlocks* create_empty_struct(int size) {
	ArrayOfBlocks * new_struct = calloc(1, sizeof(ArrayOfBlocks));
	new_struct->capacity = size;
	new_struct->amount = 0;
	new_struct->ptr = calloc(size, sizeof(char*));
	new_struct->deleted = calloc(size, sizeof(bool));
	return new_struct;
}

void destroy(ArrayOfBlocks * struct_to_destroy) {
	for (int i = 0; i < struct_to_destroy->amount; i++) {
		if (!struct_to_destroy->deleted[i]) {
			free(struct_to_destroy->ptr[i]);
		}
	}
	memset(struct_to_destroy->deleted, false, sizeof(bool)*(struct_to_destroy ->capacity));	
}



char* get_output(const char *filename) {
	FILE* file = fopen(filename, "r");
	//seek for file size https://stackoverflow.com/questions/238603/how-can-i-get-a-files-size-in-c
	fseek(file, 0, SEEK_END);
	int size = ftell(file);
	fseek(file, 0, SEEK_SET);

	//load content to buffer
	//https://stackoverflow.com/questions/174531/how-to-read-the-content-of-a-file-to-a-string-in-c
	char* buff = calloc(size, sizeof(char));
	if (buff == NULL) {
		fprintf(stderr, "Allocation failed2");
		return "";
	}
	if(fread(buff, sizeof(char), size, file) != size) {
		fprintf(stderr, "Read error");
	}
	fclose(file);
	return buff;

}




void count_lines_and_words(ArrayOfBlocks * curr_struct, const char* file_name) {
	// assume that filename is a path to file
	char* tmp_file = "tmp_result";
	char* wc_command = "wc -wlm ";
	char* save_to_file_command = " > ";
	
	char* res_command = malloc(strlen(file_name) + strlen(wc_command) + strlen(save_to_file_command) + strlen(tmp_file) + 1);

	if (res_command == NULL) {
		fprintf(stderr, "allocation failed");
		return;
	}

	// copy the command to result string
	strcpy(res_command, wc_command);

	// concatenate the file name to the result string
	strcat(res_command, file_name);
	
	// concatenate save symbol to res_command
	strcat(res_command, save_to_file_command);
	
	//concatenate tmp_file to res_command
	strcat(res_command, tmp_file);

	//process the command
	if (system(res_command) == -1) {
		fprintf(stderr, "command execution failed");
		return;
	}
	char* output = get_output(tmp_file);
	if (strlen(output) == 0) {
		fprintf(stderr, "Error while reading the file");
		return;
	}
	// remove tmp file
	
	char* rm_command = "rm -f ";
	res_command = realloc(res_command ,strlen(rm_command) + strlen(tmp_file) + 1);
	if (res_command == NULL) {
		fprintf(stderr, "Allocation failed3");
		return;
	}
	strcpy(res_command, rm_command);
	strcat(res_command, tmp_file);
	// TODO: Consider to use snprintf
	if(system(res_command) == -1) {
		fprintf(stderr, "command execution failed");
	}

	// write results to structure
	
	if (curr_struct->capacity < curr_struct->amount + 1) {
		fprintf(stderr, "Not enough space");
		return;
	}
	curr_struct->ptr[curr_struct->amount] = output;
	curr_struct->deleted[curr_struct->amount] = false;

	curr_struct->amount++;
	
		
}


char* show_at(ArrayOfBlocks * curr_struct, int index) {
	if (curr_struct->capacity <= index) {
		fprintf(stderr, "index out of range");
		return "";
	}
	if (curr_struct->deleted[index] == true) {
		fprintf(stderr, "memory at this point is deallocated");
		return "";
	}

	return curr_struct->ptr[index];	
}

void remove_at(ArrayOfBlocks * curr_struct, int index) {
	
	if (curr_struct->capacity <= index) {
		fprintf(stderr, "index out of range");
		return;
	} 

	if (curr_struct->deleted[index]) {
		fprintf(stderr, "index already removed");
		return;
	} 
	free(curr_struct->ptr[index]);
	curr_struct->deleted[index] = true;

}

