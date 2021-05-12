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

		/* Break when encountering redirection character */
		/*if (*ptr == '>') {
			printf("parsing: redirection\n");
			break;
		}*/

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
	int iFileOutput;
	int i = 0;
	int a;

	if ((pid = fork()) == -1) {
		fprintf(stderr, "forking error\n");
		exit(1);
	}

	pathOutput = checkRedirection(arg, argCount);

	if (pid == 0) {
		strcpy(program, &arg[0][0]);

		if ((access(&path[i][0], X_OK)) != 0) {
			//write(STDERR_FILENO, ERROR_MESSAGE, strlen(ERROR_MESSAGE)); 
			exit(1);
		}

		strcat(&path[i][0], program);


		/* Redirection */
		if (pathOutput != NULL) {
			argCount -= 2;

			printf("%s\n", path[i]);

			/* Output redirection, reference:
			https://stackoverflow.com/questions/8516823/redirecting-output-to-a-file-in-c */
			if ((iFileOutput = open(pathOutput, O_CREAT | O_WRONLY | O_TRUNC, 0600)) == -1) {
				fprintf(stderr, "error opening file for redirection\n");
			}
			if (dup2(iFileOutput, fileno(stdout)) == -1) {
				fprintf(stderr, "error with output redirection\n");
			}
			close(iFileOutput);

			for (a = argCount; a < sizeof(arg); a++) {
				free(arg[a]);
				arg[a] = NULL;
			}

		} 

		/* execv() replaces the current running process with a new process */  	
		if (execv(&path[i][0], arg) == -1) {
			exit(1);
			printf("jee\n");
		}


		for(a=0; a<argCount; a++) {
			// TODO fix issue with multiple commands
		}
	}
	else {
		waitpid(pid, &status, 0);
		exitStatus = WEXITSTATUS(status);
		if (exitStatus != 0) {
			write(STDERR_FILENO, ERROR_MESSAGE, strlen(ERROR_MESSAGE)); 
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
