#include "main.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define _BUFFER_SIZE 256
static char BUFFER[_BUFFER_SIZE];

Table *create_table() {
	Table *table = malloc(sizeof(Table));
	table-> capacity = 4;
	table->amount = 0;
	table->blocks = calloc(table->capacity, sizeof(Block));
	return table;
}

