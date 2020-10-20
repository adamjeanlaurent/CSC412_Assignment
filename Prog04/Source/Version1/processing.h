#ifndef PROCESSING_H
#define PROCESSING_H

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
char* process(ListOfLines* listOfLines);
int sortingByAscendingFunction(const void* a, const void* b);

#endif