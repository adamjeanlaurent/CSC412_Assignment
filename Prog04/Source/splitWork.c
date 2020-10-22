// system includes
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// custom includes
#include "splitWork.h"

int* getIndexRangesForProcesses(int numOfFiles, int numOfProcess) 
{
    int* processIndexes = (int*)calloc(numOfProcess, sizeof(int)); 

    int i;
    
    if(numOfFiles % numOfProcess == 0)
    {
        for(i = 0; i < numOfProcess; i++)
        {
            processIndexes[i] = numOfFiles/numOfProcess;
        }
    }

    else
    {
        int upTo = numOfProcess - (numOfFiles % numOfProcess);
        int quotient = numOfFiles/numOfProcess;
        for(i = 0; i < numOfProcess; i++)
        {
            if(i >= upTo)
            {
                processIndexes[i] = quotient + 1;
            }
            else
            {
                processIndexes[i] = quotient;
            }
        
        }
    }   
    return processIndexes;
}

Array2D** splitWork(int numOfFiles, int numOfProcess, Array2D* fileList) 
{
    // dynamically allocate memory for array of Array2D
    Array2D** workLists = (Array2D**)calloc(numOfProcess, sizeof(Array2D*));

    // get int array of num of filenames each Array2D should hold
    int* processingIndexes = getIndexRangesForProcesses(numOfFiles, numOfProcess);

    int i;
    int j;
    int soFar = 0;
     
    // loop through p indexes
    // insert into i-th list, files in range(upto + i)
    
    // loop through num of processes we have
    for(i = 0; i < numOfProcess; i++)
    {
        workLists[i] = (Array2D*)malloc(sizeof(Array2D));
        // the num of files that the ith 2D Array should hold
        int numOfFiles = processingIndexes[i];
        workLists[i]->rows = numOfFiles;

        // insert into worksLists[i] filenames numOfProcess[i], starting at soFar

        // dynamically allocating memory for current 2DArray->contents
        workLists[i]->contents = (char**)calloc(numOfFiles ,sizeof(char*));

        // insert into contents
        // loop through num of file the ith 2D Array should hold
        for(j = 0; j < numOfFiles; j++)
        {
            // filename of file to add right now
            char* fileToAdd = fileList->contents[soFar];
            
            // dynamically allocate memory for jth row of workLists ith list
            // use size of filename + 1
            workLists[i]->contents[j] = (char*)calloc(strlen(fileToAdd) + 1, sizeof(char));
            workLists[i]->contents[j][strlen(fileToAdd)] = '\0';

            // append fileName into correct bucket
            sprintf(workLists[i]->contents[j], "%s", fileToAdd);

            // increment soFar 
            soFar++;
        }
    }
    
    free(processingIndexes);
    return workLists;
}