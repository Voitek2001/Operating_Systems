#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void write_email(const char* email, const char* sub, const char* content ) {
	
	FILE* mail_handler;
	char command[256];
	sprintf(command, "mail -s %s %s", sub, email);
	mail_handler = popen(command, "w");
	fputs(content, mail_handler);
	pclose(mail_handler);

}

typedef enum PRINT_TYPE {
	SENDER,
	DATE,
	UNKNOWN
} PRINT_TYPE;


void print_info(PRINT_TYPE curr_type) {
	FILE* mail_handler;
	if (curr_type == SENDER) {
		mail_handler = popen("mail | tail -n +2 | sort -k3d -", "w");
	} else {
		mail_handler = popen("mail | tail -n +2", "w");
	}
	fputs("exit", mail_handler);
	pclose(mail_handler);
}


int main(int argc, char** argv) {
	
	if (argc != 2 && argc != 4) {
		fprintf(stderr, "Error");
		return 1;
	}
	if (argc == 4) {
		const char* email = argv[1];
		const char* sub = argv[2];
		const char* content = argv[3];
		write_email(email, sub, content);
	}
	if (argc == 2) {
		PRINT_TYPE curr_type = UNKNOWN;
		if (strcmp(argv[1], "nadawca") == 0) {
			curr_type = SENDER;
		}
		if (strcmp(argv[1], "data") == 0) {
			curr_type = DATE;
		}
		if (curr_type == UNKNOWN) {
			fprintf(stderr, "Usage error");
			exit(EXIT_FAILURE);
		}
		print_info(curr_type);

	}	
	
	return 0;
}
