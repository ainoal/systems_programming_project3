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
#include <fcntl.h>
#include "wish.h"

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

char *checkRedirection(char **arg, int argCount) {
	int i;
	char *pathOutput = NULL;

	for (i=0; i<argCount; i++) {	
		if (arg[i][0] == '>') {
			pathOutput = arg[i + 1];
		}
	}
	return pathOutput;
}

/* Function to execute other than built-in commands */
void executeCommand(char **arg, int argCount, char path[10][50]) {
	pid_t pid;
	int status;
	int exitStatus;
	char program[50];
	char *pathOutput = NULL;
	FILE *fileOutput;
	int i;

	if ((pid = fork()) == -1) {
		fprintf(stderr, "forking error\n");
		exit(1);
	}

	pathOutput = checkRedirection(arg, argCount);
	    if (pathOutput != NULL) {
	        if ((fileOutput = fopen(pathOutput, "w")) == NULL) {
	            printf("file opening error\n");
	        }
			argCount -= 2;
	    }

	if (pid == 0) {
		strcpy(program, &arg[0][0]);

		for(i=0; i<argCount; i++) {
			
			if (pathOutput == NULL) {
				if ((access(&path[i][0], X_OK)) != 0) {
					//write(STDERR_FILENO, ERROR_MESSAGE, strlen(ERROR_MESSAGE)); 
					exit(1);
				}

				strcat(&path[i][0], program);

				/* execv() replaces the current running process with a new process */  	
				if (execv(&path[i][0], arg) != -1) {
					exit(0);
				}
			}

			/* If next "arg" is redirection, redirect the output */
			if ((strcmp(arg[i+2], pathOutput) == 0) && pathOutput != NULL) {
				strcat(&path[i][0], program);

////////////////////////////////////////////////////////////////////////////////////
				// https://stackoverflow.com/questions/8516823/redirecting-output-to-a-file-in-c
				// replace cout.log with output file
				// replace error file with what???

				int out = open("cout.log", O_RDWR|O_CREAT|O_TRUNC, 0600);
				if (-1 == out) { perror("opening cout.log");  }

				int err = open("cerr.log", O_RDWR|O_CREAT|O_TRUNC, 0600);
				if (-1 == err) { perror("opening cerr.log"); }

				int save_out = dup(fileno(stdout));
				int save_err = dup(fileno(stderr));

				if (-1 == dup2(out, fileno(stdout))) { perror("cannot redirect stdout");  }
				if (-1 == dup2(err, fileno(stderr))) { perror("cannot redirect stderr");  }

				puts("Heyo?");

				fflush(stdout); close(out);
				fflush(stderr); close(err);

				dup2(save_out, fileno(stdout));
				dup2(save_err, fileno(stderr));

				close(save_out);
				close(save_err);

				puts("back to normal output");

///////////////////////////////////////////////////////////////////////////////////////

				printf("ONNISTUIN\n");
				exit(0);
			} 
		}

		printf("tää error?\n");
        exit(1);
	}
	else {
		waitpid(pid, &status, 0);
		exitStatus = WEXITSTATUS(status);
		if (exitStatus != 0) {
			write(STDERR_FILENO, ERROR_MESSAGE, strlen(ERROR_MESSAGE)); 
			printf("tämä error?\n");
		}
	}
}

/* The built-in command cd changes directories. It takes exactly one argument,
which specifies the directory where to move. */
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
			write(STDERR_FILENO, ERROR_MESSAGE, strlen(ERROR_MESSAGE)); 
		}
	}
}

/* The built-in command path changes the path(s) where wish looks for 
executable programs. */
void setPath(int argCount, char **arg, char path[10][50]) {
	int i;

	if (argCount > 10) {
		fprintf(stderr, "Please give maximum 10 paths!\n");
	} 
	else {
		for (i=1; i<argCount; i++) {
			strcpy(&path[i-1][0], arg[i]);
			strcat(path[i-1], "/");
		}
	}
}

/*******************************************************************/
/* eof */
