#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "processing.h"

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

Array2D** distributeToDoLists(Array2D** workLists, int numOfProcess)
{
    Array2D** toDoLists = (Array2D**)calloc(numOfProcess, sizeof(Array2D*));

    int i;
    int j;
    
    // for number of processes
    for(i = 0; i < numOfProcess; i++)
    {
        // for num of files in that processes workList
        for(j = 0; j < workLists[i]->rows; j++)
        {
            // read that file
            // put it in it's corresponding toDo List
            FILE *fp;
            
            // get current filename
            char* fileName = workLists[i]->contents[j];

            // open file
            fp = fopen(fileName, "r");

            if(fp == NULL)
            {
                exit(1);
            }
            int index;

            // get index number of file
            fscanf(fp, "%d", &index);
            fclose(fp);

            // now that we have the index, put it in the corresponding list

            // if we haven't allocated anything for this list yet
            // allocate it
            if(toDoLists[index] == NULL)
            {
                // allocate Array2D pointer 
                toDoLists[index] = (Array2D*)malloc(sizeof(Array2D));

                // allocate memeory for char** contents pointers
                toDoLists[index]->contents = (char**)calloc(1 ,sizeof(char*));

                toDoLists[index]->contents[0] = (char*)calloc(strlen(fileName) + 1, sizeof(char));

                toDoLists[index]->contents[0][strlen(fileName)] = '\0';

                toDoLists[index]->rows = 1;

                // append fileName into correct bucket
                sprintf(toDoLists[index]->contents[0], "%s", fileName);
            }

            // if that to do list already exists
            // realloc contents pointer
            // allocate contents[ row + 1]
            else 
            {   
                int row = toDoLists[index]->rows;
                toDoLists[index]->contents = (char**)realloc(toDoLists[index]->contents ,sizeof(char*) * (row + 1));
                toDoLists[index]->contents[row] = (char*)calloc(strlen(fileName) + 1, sizeof(char));
                toDoLists[index]->contents[row][strlen(fileName)] = '\0';
                sprintf(toDoLists[index]->contents[row], "%s", fileName);
                toDoLists[index]->rows++;
            }
        }
    }
    return toDoLists;
}

