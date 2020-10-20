#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "processing.h"
#include "distributor.h"

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

ListOfLines* collectResultsFromDistribution(int numOfProcess, char* tempDir)
{
    // create n list of lines (what we are going to return)
    ListOfLines* listOfLines = (ListOfLines*)malloc(sizeof(ListOfLines) * numOfProcess);
    int i;

    // init ListOfLines
    for(i = 0; i < numOfProcess; i++)
    {
        listOfLines[i].length = 0;
        listOfLines[i].lines = (Line*)malloc(sizeof(Line)); // init each list of lines with 1 line
        listOfLines[i].lines[0].index = 0;
        listOfLines[i].lines[0].lineNum = 0;
        listOfLines[i].lines[0].contents = NULL;
    }

    for(i = 0; i < numOfProcess; i++)
    {
        // get to do list file name
        char* toDoListFilePath = generateToDoListFileName(tempDir, i);

        // open to do list
        FILE* toDoListFP = fopen(toDoListFilePath ,"r");

        // if file exists
        if(toDoListFP != NULL)
        {
            // read each line in toDoList
            char fragmentFilePath[500];
            while(fscanf(toDoListFP, "%s", fragmentFilePath) == 1)
            {
                // open fragment and store results
                FILE* fragmentFP = fopen(fragmentFilePath, "r");
                // if fragment exists
                if(fragmentFP != NULL)
                {
                    // read one line from file
                    int index;
                    int lineNum;
                    char lineBuffer[500];

                    fscanf(fragmentFP, "%d %d", &index, &lineNum);
                    fgets(lineBuffer, 500, fragmentFP);

                    // store in object
                    int n = listOfLines[i].length; // i = current list we are looking at
                    listOfLines[i].lines[n].index = index;
                    listOfLines[i].lines[n].lineNum = lineNum;
                    listOfLines[i].lines[n].contents = (char*)calloc(strlen(lineBuffer) + 1, sizeof(char));
                    sprintf(listOfLines[i].lines[n].contents, "%s", lineBuffer);
                    listOfLines[i].length++;

                    // realloc 1 more space
                    listOfLines[i].lines = (Line*)realloc(listOfLines[i].lines, listOfLines[i].length + 1);

                    fclose(fragmentFP);
                }
            }
            fclose(toDoListFP);
        }
        free(toDoListFilePath);
    }
    return listOfLines;
}

void printListOfLines(ListOfLines* list, int numOfLists)
{
    int i;
    int j;

    // for all list of lines
    for(i = 0; i < numOfLists; i++)
    {
        // print all line in list of lines
        printf("List Of Lines %d: \n", i);
        printf("len %d: \n", list[i].length);
        printf("*********************************\n");

        // print all line in list
        for(j = 0; j < list[i].length; j++)
        {
            int index = list[i].lines[j].index;
            int lineNum = list[i].lines[j].lineNum;
            char* contents = list[i].lines[j].contents;

            printf("Line %d: \n", j);
            printf("index: %d \n", index);
            printf("line number: %d \n", lineNum);
            printf("contents: %s \n", contents);
            printf("--------------------\n");
        }
        printf("*********************************\n");
    }
}