/*******************************************************************/
/* CT30A3370 Käyttöjärjestelmät ja systeemiohjelmointi
 * wish.c
 * Aino Liukkonen
 * 24.4.2021
 */
/*******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "wish.h"

int main(int argc, char *argv[]) {
	char **arg = NULL;	
	char *buffer = NULL;
	size_t bufferSize = 0;
	ssize_t lineLen;
	
	switch (argc) {
		case 1:
			while (1) {
				printf("wish> ");

				if ((lineLen = getline(&buffer, &bufferSize, stdin)) != -1) {
					if ((arg = calloc(MAX_ARGS, sizeof(__intptr_t))) == NULL) {
						perror("Calloc error\n"); // ???
					}
					parse(buffer, arg);
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
