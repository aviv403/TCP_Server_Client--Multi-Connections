#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <arpa/inet.h>    //close
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/un.h>


#define MAX_STR_SIZE 256
#define BUFSIZE 1000
#define TRUE   1
#define FALSE  0
// #define PORT 8888

void append(char *str, char c);
void appendy(char *str, char c);
void printMemo(char **A, int size);
void freeMemo(char **arr, int size);
bool isPath(char * path);
int isIdentifier(char *id);
int allocMemory(char *msg, char ***seprPath);
int separatePath(char *msg, char **seprPath, int size);
char *mkRealPath(char *msg, char* p);
int firstResponse(char *msg, char* p);
char *secondResponse(char * msg, char *p);
void thirdResponse(char *msg, char *p);
void findpaths(char **paths, char *p);
int is_regular_file(const char *path);
int readInt(char *path);
int fourthResponse(char *msg, char *p);
char *fifthResponse(char *msg, char *p);
int whichCommand(char *msg);

char isFoundInFile(char *msg, char *p);
void appendToFile(char *msg, char *id, char *p);
void removeLinefromFile(char *msg, int line, char* p);
int findLine(char *msg, char *p);
char *getId(char *msg, char *p);
int sumIntegers(char *msg, char *p);
int countlines(FILE * fin);


#endif
