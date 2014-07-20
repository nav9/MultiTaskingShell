#ifndef _JOBPROC_H
#define _JOBPROC_H

#include<sys/types.h>
#include<termios.h>
#include<unistd.h>
#include<signal.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<sys/wait.h>
#include<string.h>
#include<time.h>
#define DELIM "\n\r\t ;"

typedef struct process
{
 int startTime;
 int pid;
 int pgid;
 int running; 
} process;

void addProcess(int ,int ,int ,int);
void assignProcesses(char**, int, int);
void displayProcesses();
void parsep(char*, char **);
void redirect(char *, char *,int);
void initializeShell();

#endif
