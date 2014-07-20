#include"jobproc.h"
#include"sigj.h"

struct process pList[50];//50 is the maximum number of simultaneous processes supported
int maxProcesses=50;
int existingProcesses=0;
int latestPID=0;
pid_t shellPgid;
struct termios shellTerminalAttributes;
int shellTerminal;

void assignProcesses(char **s, int maxProcesses, int fg)
{
 int p=0,ppid=0,pid=0,foreground=1;
 int fd[2];
 int stat;
 pipe(fd);
 char *ps1[40],*ps2[40];
 parsep(s[0],ps1);
 parsep(s[1],ps2);
 //0 is STDIN, 1 is STDOUT

 int pgid=getpid();
 int success=0,invalid=0;

 if ((pid=fork())==0)
    {
     if (setpgid(pid, pgid)<0) write(STDOUT_FILENO,"Couldn't assign separate group id\n",34); 
     if (fg==1) 
        { 
         if (tcsetpgrp(shellTerminal, pgid)<0) write(STDOUT_FILENO,"Couldn't put in separate process group\n",40);
        }
        signal(SIGINT,overideInt);//set the Ctrl-C signal handler
        signal(SIGTSTP,overideSuspend);//set the Ctrl-Z signal handler
        signal(SIGQUIT,overideQuit);//set the Ctrl-\ signal handler

     dup2(fd[1],1);
     if ((invalid=execvp(ps1[0],ps1))<0) {write(STDOUT_FILENO,"Invalid command\n",16);exit(1);}
    }//if
 else 
 {
  close(fd[1]);
  if (invalid>=0) {latestPID=pid;addProcess(pid,pgid,1,0);}
  if (fg==1) waitpid(pid,&stat,0);
  else 
  {
   if (invalid>=0)
   {
    char num[10];intToAscii((int)pid, num);
    write(STDOUT_FILENO,num,6);
    write(STDOUT_FILENO," process sent to background\n",28);
   }//if
  }//else
 }//else

 if ((pid=fork())==0)
    {
     if (setpgid(pid, pgid)<0) write(STDOUT_FILENO,"Couldn't assign separate group id\n",34); 
     if (fg==1) 
        {if (tcsetpgrp(shellTerminal, pgid)<0) write(STDOUT_FILENO,"Couldn't put in separate process group\n",40);}
     signal(SIGINT,overideInt);//set the Ctrl-C signal handler
     signal(SIGTSTP,overideSuspend);//set the Ctrl-Z signal handler
     signal(SIGQUIT,overideQuit);//set the Ctrl-\ signal handler
     
     dup2(fd[0],0);
     if ((invalid=execvp(ps2[0],ps2))<0) {write(STDOUT_FILENO,"Invalid command\n",16);exit(1);}
    }//if
 else 
    {
     if (invalid>=0) {latestPID=pid;addProcess(pid,pgid,1,0);}
     if (fg==1) waitpid(pid,&stat,0);
     else 
     {
      if (invalid>=0)
      {
       char num[10];intToAscii((int)pid, num);
       write(STDOUT_FILENO,num,6);write(STDOUT_FILENO," process sent to background\n",28);}
      }
    }//else
}//assignProcess

void addProcess(int id,int gid,int run,int tim)
{
 int i=0,found=0;
 pList[existingProcesses].startTime=tim;pList[existingProcesses].pid=id;
 pList[existingProcesses].pgid=gid;pList[existingProcesses].running=run;
 if (existingProcesses<maxProcesses-1) existingProcesses++; 
 else 
 {
  for(i=0;i<existingProcesses;i++)
  {
   if (pList[i].running==0)
      {
       pList[existingProcesses].startTime=tim;
       pList[existingProcesses].pid=id;
       pList[existingProcesses].pgid=gid;
       pList[existingProcesses].running=run;
      }
  }//for
  if (found==0) write(STDOUT_FILENO,"Process limit reached\n",22);
 }//else
}//addProcess

void displayProcesses()
{
 int i=0;
 if (existingProcesses==0) write(STDOUT_FILENO,"No processes were run\n",22);
 else
 {
 for(i=0;i<existingProcesses;i++)
 {
  char num[10];
  intToAscii(pList[i].pid, num);
  write(STDOUT_FILENO,"Processes that were run:\n",25);
  write(STDOUT_FILENO,num,strlen(num));write(STDOUT_FILENO,"= process id\n",13);
  //intToAscii(pList[i].running, num);
  //write(STDOUT_FILENO,num,strlen(num));write(STDOUT_FILENO,"=status\n",8);
 }//for
 }//else
}//displayProcesses

void setLatestPID(int i) {latestPID=i;}//setLatestPID
int getLatestPID() {return latestPID;}//getLatestPID
void killLatest() 
{
 if (existingProcesses==0) write(STDOUT_FILENO,"No processes to kill\n",21);
 else
 {
  char num[10];intToAscii(pList[existingProcesses-1].pid, num);
  if (existingProcesses!=0) existingProcesses--;
  write(STDOUT_FILENO,num,6);write(STDOUT_FILENO," process has been issued kill signal. May already be killed\n",61);
  kill(latestPID,SIGKILL);
 }//else
}//killLatest

/*void redirect(char *s, char *fileName,int bg)
{
 int pid;
 int fd[2];
 char *ps[40]={NULL},*fl[40];
 pipe(fd);
 parsep(s,ps);
 //parsep(fileName,fl);
  fd[0]=creat(fl[0],00600);
  dup2(fd[1],1);

 if ((pid=fork())==0)
 {
  if (execvp(ps[0],ps)<0)
     {
      write(STDOUT_FILENO,"1Invalid command\n",16); 
      exit(1);
     }
 }//if
 else 
 {  
  close(fd[0]);close(fd[1]);
 }
 write(STDOUT_FILENO,"2Invalid command\n",16); 
}//redirect*/

void parsep(char *inp, char **argv)
{
 *argv=strtok(inp, DELIM);
 *argv++;inp++;
 while((*argv= strtok(NULL,DELIM))!=NULL) *argv++;
 *argv=NULL;
}//parse

void initializeShell()
{
 shellTerminal=STDIN_FILENO;
 printf("Terminal id=%d, pgid=%d\n\n",tcgetpgrp(shellTerminal),getpgrp());
 while(tcgetpgrp(shellTerminal) != (shellPgid=getpgrp())) kill(- shellPgid,SIGTTIN);
 printf("Initializing shell...\n");
 signal(SIGTTIN,SIG_IGN);signal(SIGTTOU,SIG_IGN);signal(SIGCHLD,SIG_IGN);
 signal(SIGINT,overideInt);//set the Ctrl-C signal handler
 signal(SIGTSTP,overideSuspend);//set the Ctrl-Z signal handler
 signal(SIGQUIT,overideQuit);//set the Ctrl-\ signal handler
 printf("3 Signals ignored. 3 Signals Handled\n");

 shellPgid=getpid();//Put shell in its own process group.
 if (setpgid(shellPgid,shellPgid)<0) {printf("Couldn't put the shell in its own process group\n");exit(1);}
 else printf("Shell in control. Taking control of the terminal...\n\n");
 tcsetpgrp(shellTerminal, shellPgid);//Grab control of the terminal.
 tcgetattr(shellTerminal, &shellTerminalAttributes);//Save default terminal attributes for shell.
}//initializeShell
