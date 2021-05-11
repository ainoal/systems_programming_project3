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
void executeCommand(char **arg, int argCount, char path[10][50]);
int **freeArgs(char **arg, int argCount);
int checkRedirection(char **arg, int argCount);
void cd(int argCount, char *path);
void setPath(int argCount, char ** arg, char path[10][50]);

/*******************************************************************/
/* eof */
