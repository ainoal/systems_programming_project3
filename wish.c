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
	
	switch (argc) {
		case 1:
			while (1) {
				printf("wish> ");

				if ((lineLen = getline(&buffer, &bufferSize, stdin)) != -1) {
					if ((arg = calloc(MAX_ARGS, sizeof(__intptr_t))) == NULL) {
						perror("Calloc error\n"); // ???
					}
					argCount = parse(buffer, arg);

					if (strcmp(arg[0], "exit") == 0) {
						exit(0);
					} 
					else if (strcmp(arg[0], "cd") == 0) {
						printf("execute cd\n");
						// execute cd command
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
		case 2:
			// batch mode
			break;
		default: 	// If there are more than 1 command line arguments
			printf("Usage: ./wish [batch-file]\n"); 
			exit(1);
	}

	return 0;
}


/*******************************************************************/
/* eof */
