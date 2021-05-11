/*******************************************************************/
/* CT30A3370 Käyttöjärjestelmät ja systeemiohjelmointi
 * wishlib.c
 * Aino Liukkonen
 * 24.4.2021
 */
/*******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include "wish.h"

char *pathError = NULL;
char *pathOutput = NULL;

/* Function parse() parses through a line and finds where each argument 
starts and ends to make the code robust to whitespace of various kinds */
int parse(char *ptr, char **arg) {
    int count;
    int i = 0;
    int argCount = 0;

    while (*ptr != '\0' && *ptr != '\n') {
        count = 0;

		/* Move 'past' whitespace characters */
        while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n') {
            *ptr++ = '\0';
        }

		/* While pointer points to a non-whitespace character */
        while (*ptr != ' ' && *ptr != '\t' && *ptr != '\0' && *ptr != '\n') {
            count++;	// Count the number of characters in the argument
            ptr++;		// Move pointer to the next character
        }

        if (i > MAX_ARGS) {
			fprintf(stderr, "you gave too many arguments\n");
            exit(1);
        }
		if ((arg[i] = calloc(count, sizeof(__intptr_t))) == NULL) {
			fprintf(stderr, "calloc failed\n");
			exit(1);
		}								

		/* Move pointer to the start of the argument and copy memory area */
        ptr -= count;
        memcpy(arg[i], ptr, count);
        
		/* Move back to the end of the argument */
		ptr += count;
        i++;
    }
    argCount = i;

	return argCount;
}

void executeCommand(char **arg, int argCount, char path[10][50]) {
	pid_t pid;
	int status;
	int exitStatus;
	//char path[10][50];
	char program[50];
	int i;

	if ((pid = fork()) == -1) {
		fprintf(stderr, "forking error\n");
		exit(1);
	}

   if (pid == 0) {

		strcpy(program, &arg[0][0]);
		strcat(&path[0][0], program);

		strcpy(program, &arg[0][0]);
		strcat(&path[1][0], program);


		/* execv() replaces the current running process with 
		a new process */
		for(i=0; i<10; i++) {
	    	if (execv(&path[i][0], arg) != -1) {
				exit(0);
			}
		}

		fprintf(stderr, "execv error\n");	// !!!
        exit(1);
	}
	else {
		waitpid(pid, &status, 0);
		// TODO access()
		exitStatus = WEXITSTATUS(status);
		if (exitStatus != 0) {
			fprintf(stderr, "an error has occurred\n");	// !!!
		}
	}
}

void cd(int argCount, char *path) {
	if (argCount <= 1) {
		printf("Too few arguments!\n");
		printf("cd always takes exactly 1 argument\n");
	} 
	else if (argCount > 2) {
		printf("Too many arguments!\n");
		printf("cd always takes exactly 1 argument\n");
	}
	else {
		if (chdir(path)) {
			printf("cd: an error has occurred\n");
		}
	}
}

/*******************************************************************/
/* eof */
