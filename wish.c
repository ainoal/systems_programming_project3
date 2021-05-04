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
#include "wish.h"

int main(int argc, char *argv[]) {
	char **arg = NULL;	
	char *buffer = NULL;
	size_t bufferSize = 0;
	ssize_t lineLen;
	int argCount;
	int i;
	char **path;
	char program[50];
	FILE *stream;

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
		/* Initialize default paths */
		/*strcpy(&path[0][0], "/bin/");
		strcpy(program, &arg[0][0]);
		strcat(&path[0][0], program);

		strcpy(&path[1][0], "/usr/bin/");
		strcpy(program, &arg[0][0]);
		strcat(&path[1][0], program);*/

		printf("wish> ");

		if ((lineLen = getline(&buffer, &bufferSize, stdin)) != -1) {
			if ((arg = calloc(MAX_ARGS, sizeof(__intptr_t))) == NULL) {
				fprintf(stderr, "calloc failed\n");
				exit(1);
			}
			argCount = parse(buffer, arg);

			if (strcmp(arg[0], "exit") == 0) {
				exit(0);
			} 
			else if (strcmp(arg[0], "cd") == 0) {

				//if (environment.path_set_by_user == true)
				//{
				    /* Cleanup previous path memory. */
				//    free(environment.paths - 5);
				//}
				//environment.paths = line + 5;
				//environment.path_set_by_user = true;
				/* Continue so new path variable memory will be reserved. */
				continue;

			}
			else if (strcmp(arg[0], "path") == 0) {
				printf("execute path\n");
				// execute path command
			}
			else {	// Not a built-in command
				executeCommand(arg, argCount);
			}

			for (i = 0; i <= argCount; i++) {
				free(arg[i]);
				arg[i] = NULL;
			}

			free(arg);

			free(buffer);

		} else { break;}
	}



	return 0;
}


/*******************************************************************/
/* eof */
