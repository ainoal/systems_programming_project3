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

static struct node *path;

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
            printf("You gave too many arguments\n");
            exit(1);
        }
		if ((arg[i] = calloc(count, sizeof(__intptr_t))) == NULL) {
			perror("Calloc error");
		}								// ALSO NO PERROR ON THIS COURSE WORK :D

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

void executeCommand(char **arg, int argCount) {
	pid_t pid;
	int status;
	int exitStatus;
	//char *path;
	char program[50];
	int i;

	if ((pid = fork()) == -1) {
		perror("Forking error");
		exit(1);
	}

   if (pid == 0) {
		/*if (execv(path, arg) == -1) {
			perror("Execv error");
		}*/


		//strcpy(&path[0], "/bin/");
		//strcpy(program, &arg[0][0]);
		//strcat(&path[0], program);

		//strcpy(&path[1], "/usr/bin/");
		//strcpy(program, &arg[0][0]);
		//strcat(&path[1], program);

		//int paths = 2;

		//strcpy(path2, "/usr/bin/");
		//strcpy(program, &arg[0][0]);
		//strcat(path2, program);

		/* execv() replaces the current running process with 
		a new process */
		//for (i=0; i<paths; i++) {
		//    if (execv(&path[i], arg) == -1) {
		//        perror("Execv error");
				//exit(1);
		//    }
		//}
        exit(0);
	}
	else {
		waitpid(pid, &status, 0);
		exitStatus = WEXITSTATUS(status);
		if (exitStatus != 0) {
			perror("Error with parallel commands");
		}
	}
}

void initializePath() {
    addElement(&path, "/bin");
	//addElement(&path, "/usr/bin");
}

void addElement(struct node **pHead, char* element) {
    NODE *pNew;

	pNew = malloc(sizeof(NODE));	// Add error handling (if time :D)
    pNew->val = element;
    pNew->pNext = NULL;

    if (*pHead == NULL) {
        *pHead = pNew;
    } 
	else {
        NODE *pCurrent = *pHead;
        while (pCurrent->pNext) {
            pCurrent = pCurrent->pNext;
		}
        pCurrent->pNext = pNew;
    }
}

char *findInPath(char *command) {
    NODE *pCurrent = path;
    char *res = NULL;
	size_t totalLen;
	char *str;

    while (pCurrent != NULL) {
        totalLen = strlen(pCurrent->val) + strlen(command) + 2;
        str = calloc(1, totalLen);
        memcpy(str, pCurrent->val, strlen(pCurrent->val));
        memcpy(str + strlen(pCurrent->val), "/", 1);
        memcpy(str + strlen(pCurrent->val) + 1, command, strlen(command));
        if (access(str, X_OK) == 0) {
            res = str;
            break;
        }

        pCurrent = pCurrent->pNext;
    }

    return res;
}

/*******************************************************************/
/* eof */
