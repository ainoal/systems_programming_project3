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

/* Function parse() parses through a line and finds where each argument 
starts and ends to make the code robust to whitespace of various kinds */
void parse(char *ptr, char **arg) {
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
			perror("Calloc error\n"); // ???
		}

		/* Move pointer to the start of the argument and copy memory area */
        ptr -= count;
        memcpy(arg[i], ptr, count);
        
		/* Move back to the end of the argument */
		ptr += count;
        i++;
    }
    argCount = i;
}

/*******************************************************************/
/* eof */
