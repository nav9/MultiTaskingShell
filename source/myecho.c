#include "myecho.h"
int myecho(char *ac[])
{
        int i=1;
        while(ac[i]!=NULL)
        {
                printf("%s ",ac[i]);
                i++;
        }
        printf("\n");
}

