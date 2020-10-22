#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "processing.h"
#include "distributor.h"

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
            while(fscanf(toDoListFP, "%s ", fragmentFilePath) == 1)
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
                    // sprintf(listOfLines[i].lines[n].contents, "%s", lineBuffer);
                    lineBuffer[strcspn(lineBuffer, "\n")] = 0;
                    strcpy(listOfLines[i].lines[n].contents, lineBuffer);
                    // puts(listOfLines[i].lines[n].contents);
                    listOfLines[i].lines[n].contents[strlen(listOfLines[i].lines[n].contents)] = '\0';
                    
                    listOfLines[i].length++;

                    // realloc 1 more space
                    listOfLines[i].lines = (Line*)realloc(listOfLines[i].lines, (listOfLines[i].length + 1) * sizeof(Line));
                    //printf("i: %d n: %d \n %s\n",i,n,listOfLines[i].lines[n].contents);
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
     printf("number Of lists %d: \n", numOfLists);
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
            printf("contents: %s\n", contents);
            printf("--------------------\n");
        }
        printf("*********************************\n");
    }
}

char* processV1(ListOfLines* listOfLines)
{
    // sort elements
    int numLines = listOfLines->length;
    qsort(listOfLines->lines, numLines, sizeof(Line), sortingByAscendingFunction);

    // get length needed
    int lengthOfLongString = 1 + (numLines * 2); // init with one for \0 , num lines for \n for each line 
    int i;

    for(i = 0; i < numLines; i++)
    {
        lengthOfLongString += strlen(listOfLines->lines[i].contents);
    }

    // allocate string to hold long string
    char* longString = (char*)calloc(lengthOfLongString, sizeof(char));

    // append to longString
    for(i = 0; i < numLines; i++)
    {
        strcat(longString, "\n");
        strcat(longString, listOfLines->lines[i].contents);
    }
    return longString;
}

void processV2(ListOfLines* ListOfLines, char* tempDir, int processNum)
{   
    int i;
    // sort elements 
    int numOfLines = ListOfLines->length;
    qsort(ListOfLines->lines, numOfLines, sizeof(Line), sortingByAscendingFunction);

    // write to output file

    // create output file for fragment
    char* fragmentFilePath = generateSourceFragmentFileName(tempDir, processNum);

    FILE* fp = fopen(fragmentFilePath, "a");
    
    if(fp != NULL)
    {
        // write number of lines to file
        fprintf(fp, "%d\n", numOfLines);

        // in loop write to file
        for(i = 0; i < numOfLines; i++)
        {
            fprintf(fp, "%s\n", ListOfLines->lines[i].contents);
        }

        fclose(fp);
    }
    free(fragmentFilePath);
}

int sortingByAscendingFunction(const void* a, const void* b)
{
    const Line* l1 = (Line*)a;
    const Line* l2 = (Line*)b;

    return(l1->lineNum - l2->lineNum);
}

char* generateSourceFragmentFileName(char* tempDir, int processNum)
{
    int lengthOfProcessNum = fastLengthOfNumber(processNum);
    char* fragmentFileName = "Fragment_";

    int len = strlen(tempDir) + lengthOfProcessNum + strlen(fragmentFileName) + 5; 
    char* fragmentFilePath = (char*)calloc(len, sizeof(char));
    sprintf(fragmentFilePath, "%s%s%d.txt", tempDir, fragmentFileName, processNum);
    return fragmentFilePath;
}