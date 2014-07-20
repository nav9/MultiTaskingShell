#include "sigj.h"
//the Ctrl-C handler
void overideInt()
{
        char answer[30];
        printf("\nConfirm Kill? [y/n]: ");// prompt the user to tell us if to really exit or not 
        fgets(answer,sizeof(answer),stdin);
                if (answer[0]=='y' || answer[0]=='Y') 
                {
                 killLatest();
                 //printf("\nExiting...\n");
                 //exit(0);
                }//if
                else
                printf("\nContinuing...\n");
 signal(SIGINT,overideInt);

}//CatchInt

//the Ctrl-Z handler
void overideSuspend()
{
        char answer[30];
        //printf("\nConfirm Suspend? [y/n]: ");// prompt the user to tell us if to really exit or not 
        printf("Suspend currently not supported for processes\n");
        /*fgets(answer, sizeof(answer), stdin);
                if (answer[0]=='y' || answer[0]=='Y')
                {
                   //stopLatest();
                   printf("\nSuspend...\n");
                   exit(0);
                }//if
                else
                printf("\nContinuing...\n");*/
 signal(SIGTSTP,overideSuspend);
}//overideSuspend

// the Ctrl-\ handler
void overideQuit()
{
        char answer[30];
        printf("\nConfirm Quit? [y/n]: ");// prompt the user to tell us if to really quit or not
        fgets(answer, sizeof(answer), stdin);
        if (answer[0]=='y' || answer[0]=='Y') 
        {
                printf("\nQuitting...\n");
                 exit(0);
        }//if

        else 
                printf("\nContinuing...\n");
 signal(SIGQUIT,overideQuit);
}//overideQuit

//void overideExitShell() {exit(0);}//overideExitShell
