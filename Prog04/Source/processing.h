#ifndef PROCESSING_V1_H
#define PROCESSING_V1_H

#include "fileSystemHandler.h"

typedef struct Line
{
   int index;
   int lineNum;
   char* contents;
} Line;

typedef struct ListOfLines
{
    int length;
    Line* lines;
} ListOfLines;

ListOfLines* collectResultsFromDistribution(int numOfProcess, char* tempDir);
void printListOfLines(ListOfLines* listsOfLines, int numOfLists);
char* processV1(ListOfLines* listOfLines);
int sortingByAscendingFunction(const void* a, const void* b);
void processV2(ListOfLines* listOfLines, char* tempDir, int processNum);
char* generateSourceFragmentFileName(char* tempDir, int processNum);

#endif