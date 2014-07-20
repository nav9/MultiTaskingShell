#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<wait.h>
#include<sys/wait.h>
#include<string.h>
#include<stdlib.h>
#include<termios.h>
#include<time.h>
#include<signal.h>
#include"sigj.h"
#define DELIM "\n\t\r ;&"

//int parseIt(char*, char**);
void parse(char *, char **);
int executeIfExternal(char**, char*, int);
void populateInternals();
void intToAscii(int, char*);
