/*******************************************************************/
/* CT30A3370 Käyttöjärjestelmät ja systeemiohjelmointi
 * wish.h
 * Aino Liukkonen
 * 24.4.2021
 */
/*******************************************************************/

/* MAX_ARGS defines the maximum number of arguments that user can give
at a time when the shell is running. It is needed for successful memory 
allocation with calloc() */
#define MAX_ARGS 30

int parse(char *line, char **arg);
void executeCommand(char **arg, int argCount);
int **freeArgs(char **arg, int argCount);
int checkRedirection(char **arg, int argCount);

/*******************************************************************/
/* eof */