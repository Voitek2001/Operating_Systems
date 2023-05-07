#ifndef __SHM__
#define __SHM__


int get_shared_mem(const char*, int);

char* attach_shared_memory(const char* filename, int size);

void detach_shared_memory(char* shared_memory);

void destroy_shared_memory(const char* filename);

#endif
