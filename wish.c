/*******************************************************************/
/* CT30A3370 Käyttöjärjestelmät ja systeemiohjelmointi
 * wish.c
 * Aino Liukkonen
 * 24.4.2021
 */
/*******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "wish.h"

int main(int argc, char *argv[]) {
	char **arg = NULL;	
	char *buffer = NULL;
	size_t bufferSize = 0;
	ssize_t lineLen;
	int argCount;
	int i;
	char path[10][50];
	FILE *stream = stdin;

	// TODO correct initial path
	/* Initialize default paths */
	strcpy(&path[0][0], "/bin/");
	//strcpy(&path[1][0], "/usr/bin/");

    if (argc > 2) {
		printf("Usage: ./wish [batch-file]\n"); 
		exit(1);
    }

	/* batch mode */
    if (argc == 2) {
        if ((stream = fopen(argv[1], "r")) == NULL) {
            fprintf(stderr, "cannot open batch file\n");
			exit(1);
        }
    }

	while (1) {

		printf("wish> ");

		if ((lineLen = getline(&buffer, &bufferSize, stdin)) != -1) {
			if ((arg = calloc(MAX_ARGS, sizeof(__intptr_t))) == NULL) {
				fprintf(stderr, "calloc failed\n");
				exit(1);
			}

			argCount = parse(buffer, arg);	// Includes the program name as arg[0]

			if (strcmp(arg[0], "exit") == 0) {
				exit(0);
			} 
			else if (strcmp(arg[0], "cd") == 0) {
				cd(argCount, arg[1]);

			}
			else if (strcmp(arg[0], "path") == 0) {
				// TODO max args -> 30
				setPath(argCount, arg, path);
			}
			else {	// Not a built-in command
				executeCommand(arg, argCount, path);
			}

			/* Free memory */
			for (i = 0; i <= argCount; i++) {
				free(arg[i]);
				arg[i] = NULL;
			}
			free(arg);
			arg = NULL;

			free(buffer);

		} else { break; }
	}



	return 0;
}


/*******************************************************************/
/* eof */
