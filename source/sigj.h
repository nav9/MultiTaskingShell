#ifndef _SIGJ_H
#define _SIGJ_H

#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<stdlib.h>
#include<sys/types.h>

void overideExitShell();
void overideInt();
void overideSuspend();
void overideQuit();

#endif
