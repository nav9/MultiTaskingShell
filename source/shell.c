#include "shell.h"

int maxInternals=100,maxWordLength=50,internalsSupported=0;
char internals[100][50];

//-------main
int main(int argc, char *args[])
{
 int i=0,n=0;
 char inp[256], condition[5];
 char *argv[64];
 strcpy(inp,"");
 printf(" ------------------------------------------------- \n");
 printf("|        Welcome to the MTS-2 Project Shell!      |\n");
 printf(" ------------------------------------------------- \n");
 initializeShell();//function in jobproc.c
 populateInternals();
 
 do
 {
  inp[0]='\0';
  write(STDOUT_FILENO,"\nShell Prompt>",14);
  n=read(STDIN_FILENO,inp,100);
  inp[n]='\0';
  int counter=0,semi=0;
  int foreOrBack=1;//this value will depend on whether there's a '&' in the comommand line
  if(!strcmp(inp,"exit\n")) exit(0);
  if(!strcmp(inp,"exit")) exit(0);
    for(i=0;i<n;i++) 
    {
     if (inp[i]==' ') counter++;
     if(inp[i]==';') semi++;
    }
  int cs=semi+counter;
  if(cs==n-1 && n>1) { write(STDOUT_FILENO,"Invalid Input\n\n",16);}	
  if (n>1 && counter!=n-1 && cs!=n-1)  
     {
     int k=0,detected=0,np=0;
     char s1[10]={'\0'};
     char s2[10]={'\0'};
		 
     int f=0;
     while(inp[f]!='\0'){f++;}

     if(inp[f-2]=='&') {foreOrBack=0;inp[f-2]=' ';}

     while(inp[k]!='\0')
	  {
	   if(inp[k]=='|') {detected=1;}//pipe detected
	   k++;
	  }
      /*int r=0,red=0;//redirection removed
	  while(inp[r]!='\0')
          {
           if(inp[r]=='>') {red=0;}//
           r++;
          }
  	if (red==1)
        {
	  char *ps1[40],*ps2[40];
          int p1=0;
          char *spa[40];
          while(inp[p1]!='>') {s1[p1]=inp[p1];p1++;}
          p1++;
          s1[p1+1]='\0';
          int p2=0;
          while(inp[p1+p2]!='\0') {s2[p2]=inp[p2+p1];p2++;}
          s2[p1+p2]='\0';
          spa[0]=s1;
          spa[1]=s2;

          redirect(s1,s2,foreOrBack);// if & found then FoB =0 
        }  
        else
	{*/
      	 if (detected==1)
         { 
          char *ps1[40],*ps2[40];
          int p1=0;
	  char *spa[40];
          while(inp[p1]!='|') {s1[p1]=inp[p1];p1++;}
          p1++;
	  s1[p1+1]='\0';
          int p2=0;
          while(inp[p1+p2]!='\0') {s2[p2]=inp[p2+p1];p2++;}
	  s2[p1+p2]='\0';
	  spa[0]=s1;
	  spa[1]=s2;
         
          assignProcesses(spa,3,foreOrBack);// if & found then foreOrBack=0 
         }
      else
         {
          parse(inp, argv);// 2nd method
          i=0;   			
          if (strcmp(argv[0],"exit")==0 ) exit(0);
          i=executeIfExternal(argv,argv[0],foreOrBack);

          if (strcmp(argv[0], "exit")==0 || strcmp(inp,"exit")==0) 
      	     {
	      strcpy(condition,"exit");
	     }//this has to be detected in the parsing section
        // }//else	
	}//if nothing | or &
     }//if user typed something
               
 }while(i!=999);//999 means that exit has been typed
 
 printf("Returning to bash shell...\n");
 exit(0);
}//end main

//------------------------------end of main

void populateInternals()//internal commands added to array
{
 strcpy(internals[internalsSupported++],"suspend");strcpy(internals[internalsSupported++],"kill");
 strcpy(internals[internalsSupported++],"times");strcpy(internals[internalsSupported++],"ulimit");
 strcpy(internals[internalsSupported++],"wait");strcpy(internals[internalsSupported++],"alias");
 strcpy(internals[internalsSupported++],"trap");strcpy(internals[internalsSupported++],"cd");
 strcpy(internals[internalsSupported++],"dirs");strcpy(internals[internalsSupported++],"echo");
 strcpy(internals[internalsSupported++],"pwd");strcpy(internals[internalsSupported++],"jobs");
}//populateInternals

void parse(char *inp, char **argv)//parses strings
{
 *argv=strtok(inp, DELIM);
 *argv++;
 inp++;
 while((*argv= strtok(NULL,DELIM))!=NULL) *argv++;
 *argv=NULL;
}//parse

int executeIfExternal(char **argv, char *a, int fg)
{
 int i=0,found=0,cmd=0,f=0,invalid=0;
	
 if (strcmp(a,"exit")==0){ write(STDOUT_FILENO,"Returning to shell\n",19);return 999;}
 for(cmd=0; cmd<internalsSupported; cmd++)
    {
     if(strcmp(a,internals[cmd])==0)
     f=1;
    }

     /*	if (strcmp(a,"suspend")==0 || strcmp(a,"kill")==0 || strcmp(a,"times")==0 || strcmp(a,"ulimit")==0
     || strcmp(a,"wait")==0 || strcmp(a,"alias")==0 || strcmp(a,"trap")==0 || strcmp(a,"cd")==0 || strcmp(a,"dirs")==0
     || strcmp(a,"echo")==0 || strcmp(a,"pwd")==0 || strcmp(a,"jobs")==0)
    	    		printf("You typed an internal command\n");*/
	if(f == 1)
	{
		printf("You typed internal cmd...\n");
		if(strcmp(a,"echo")==0) myecho(argv);
		else if(strcmp(a,"pwd")==0)
                        {
			 printf("in pwd\n");
			 mypwd();
                         printf("done\n");
			}
		else 
                     if (strcmp(a,"jobs")==0) displayProcesses();
                     else printf("command not implemented\n");
	}
 	else
    	{//if it's an external command
     	 pid_t  pid;
    	 int status,rv=1;
         //fork a child process
     	 if ((pid=fork())<0) {printf("Error: forking child process failed\n");rv=0;}
     	 else //for the child process:
         {
          if (pid==0) 
	     {
	      if ((invalid=execvp(*argv,argv))<0) 
		 {
                  write(STDOUT_FILENO,"Invalid command\n",16);
	          exit(1);
		 }
             }//if
           
           if (invalid==0) {setLatestPID(pid);addProcess(pid,pid,1,0);}
           if (fg==1) {waitpid(pid,&status,0);}
           else 
              {
               if (invalid==0)
               {
                char num[10];
                intToAscii((int)pid, num);
                write(STDOUT_FILENO,num,6);write(STDOUT_FILENO," process sent to background\n",28);
               }
              }
         }//else
     	 return rv;
    	}// end else
}//executeIfExternal

void intToAscii(int value, char *target)
{//for converting int to string. This function was obtained from the internet
  int div;
  unsigned char i=0;
  unsigned char removeZeroes=1;
  if(value<0) value *= -1;
  if(value==0) {target[0]='0';target[1]='\0';}
  else
  {
    for(div=10000; div!=0; div/=10)
    {
      char ch = (value / div) + '0';
      if(removeZeroes && ch == '0')
      {
        ;//don't do anything
      }
      else
      {if(!isdigit(ch)) break;removeZeroes=0;target[i]=ch;i++;}
      value %= div;
    }//for
    target[i]='\0';
  }//else
}//int to ascii
