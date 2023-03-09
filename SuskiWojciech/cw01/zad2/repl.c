#ifdef DLL
	#include "count_dll.h"
#else
	#include "count.h"
#endif

#include <dlfcn.h>
#include <time.h>
#include <sys/times.h>
#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <inttypes.h>




// declarate our struct global
ArrayOfBlocks * arr_struct;


typedef enum {
    INIT,
    COUNT,
    SHOW,
    DELETE,
    DESTROY,
    EXIT,
    UNKNOWN,
} INPUTS;


int starts_with(const char *a, const char *b)
{
   if(strncmp(a, b, strlen(b)) == 0) return 1;
   return 0;
}


void check_for_prefix(const char* line, INPUTS* curr_cmd, int* input_num, char** file_name) {

	if(starts_with(line, "init ")) {
		*curr_cmd = INIT;
		sscanf(line, "init %d", &*input_num);
		return;
	}
	
	if(starts_with(line, "count ")) {
		*curr_cmd = COUNT;
		sscanf(line, "count %s", &**file_name);
		return;
	}

	if(starts_with(line, "show ")) {
		*curr_cmd = SHOW;
		sscanf(line, "show %d", &*input_num);
		return;
	}
	
	if(starts_with(line, "delete index ")) {
		*curr_cmd = DELETE;
		sscanf(line, "delete index %d", &*input_num);
		return;
	}
	
	if(starts_with(line, "destroy")) {
		*curr_cmd = DESTROY;
		return;
	}
	if(starts_with(line, "exit")) {
		*curr_cmd = EXIT;
		return;
	}
	*curr_cmd = UNKNOWN;

}

void process_cmd(INPUTS curr_cmd, int input_num, char* input_file_name) {
	
	switch(curr_cmd) {
		
		case INIT:
			//destroy(arr_struct);
			arr_struct = create_empty_struct(input_num);
			break;
		case COUNT:
			count_lines_and_words(arr_struct, input_file_name);
			break;
		case SHOW: ;
			char* ans = show_at(arr_struct, input_num);
			printf("%s", ans);
			break;
		case DELETE:
			remove_at(arr_struct, input_num);
			break;
		case DESTROY:
			destroy(arr_struct);
			break;
		default:
			break;


	}
	

}

enum { NS_PER_SECOND = 1000000000 };

void sub_timespec(struct timespec t1, struct timespec t2, struct timespec *td)
{
    td->tv_nsec = t2.tv_nsec - t1.tv_nsec;
    td->tv_sec  = t2.tv_sec - t1.tv_sec;
    if (td->tv_sec > 0 && td->tv_nsec < 0)
    {
        td->tv_nsec += NS_PER_SECOND;
        td->tv_sec--;
    }
    else if (td->tv_sec < 0 && td->tv_nsec > 0)
    {
        td->tv_nsec -= NS_PER_SECOND;
        td->tv_sec++;
    }
}

int main() {


#ifdef DLL
	
void *handle = dlopen("./libcount.so", RTLD_LAZY);
if (!handle) {
	fprintf(stderr, "Error while loading dym lib");
	return 5;
}

*(void **) (&create_empty_struct) = dlsym(handle, "create_empty_struct");	
*(void **) (&count_lines_and_words) = dlsym(handle, "count_lines_and_words");
*(void **) (&show_at) = dlsym(handle, "show_at");
*(void **) (&remove_at) = dlsym(handle, "remove_at");
*(void **) (&destroy) = dlsym(handle, "destroy");

#endif

	//arr_struct = create_empty_struct(10);
	//count_lines_and_words(arr_struct, "/etc/passwd");
	bool running = true;
	char INPUT_LINE_BUFFER[1024];
	char* input_file_name;
	int input_num;
	INPUTS curr_cmd = 0;

	struct timespec time_buff_start, time_buff_end, delta;
	static struct tms st_cpu;
	static struct tms en_cpu;
	

	printf("\n Available options are: \n  init size \n  count filename \n  show index \n  delete index index \n  destroy \n");
	printf(">>>>>");
	while (running) {
	
		fgets(INPUT_LINE_BUFFER, 20, stdin);   //input from stdin stream
		INPUT_LINE_BUFFER[strcspn(INPUT_LINE_BUFFER, "\n")] = '\0';
		
		check_for_prefix(INPUT_LINE_BUFFER, &curr_cmd, &input_num, &input_file_name);
		INPUT_LINE_BUFFER[0] = '\0';
		
		if (curr_cmd ==EXIT) {
			break;
		}	
		if (curr_cmd == UNKNOWN) {
			printf("\n");
			printf(">>>>>");
			continue;
		}
		clock_gettime(CLOCK_REALTIME, &time_buff_start);
		times(&st_cpu);
		process_cmd(curr_cmd, input_num, input_file_name);
		times(&en_cpu);
		
		clock_gettime(CLOCK_REALTIME, &time_buff_end);
		
		sub_timespec(time_buff_start, time_buff_end, &delta);
		printf("Real Time: %d.%.9ld ns, User Time %jd, System Time %jd\n",
		        (int)(delta.tv_sec), delta.tv_nsec,
        		(intmax_t)(en_cpu.tms_utime - st_cpu.tms_utime),
        		(intmax_t)(en_cpu.tms_stime - st_cpu.tms_stime));


		printf(">>>>>");
	}

#ifdef DLL
dlclose(handle);
#endif	

}

