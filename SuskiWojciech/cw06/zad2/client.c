#include "common_utils.h"

#define NAME_LEN 6
char q_name[NAME_LEN];
char command[MSGMAX] = "";


void init_queue() {
	qNAME[0] = "/";
	for (int i = 1; i < NAME_LEN; i++) {
		q_name[i] = random_char();
    }
	


}

int main() {

	while(1) {
    
		fgets(command, MSGMAX, stdin);
	
	}
}
