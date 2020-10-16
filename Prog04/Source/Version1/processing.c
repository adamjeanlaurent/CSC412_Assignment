#include <stdio.h>
#include <stdlib.h>
#include "processing.h"

void splitWork(int numOfFiles, int numOfProcess) 
{
    int i;
    if(numOfFiles % numOfProcess == 0)
    {
        printf("[");
        for(i = 0; i < numOfProcess; i++)
        {
            if(i != numOfProcess - 1)
                printf("%d, ", numOfFiles/numOfProcess);
            else
                printf("%d", numOfFiles/numOfProcess);
            
        }
        printf("]\n");
    }

    else
    {
         printf("[");
        int upto_normal = numOfProcess - (numOfFiles % numOfProcess);
        int div = numOfFiles/numOfProcess;
        for(i = 0; i < numOfProcess; i++)
        {
            if(i >= upto_normal)
            {
                if(i != numOfProcess - 1)
                {
                     printf("%d, ", div + 1);
                }

                else
                {
                    printf("%d", div + 1);
                }
                
               
            }
            else
            {
                printf("%d, ", div);
            }
        
        }
        printf("]\n");
    }
    
}