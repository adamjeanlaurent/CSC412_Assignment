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

int* getIndexRangesForProcesses(int numOfFiles, int numOfProcess);
Array2D** splitWork(int numOfFiles, int numOfProcess, Array2D* fileList);
ListOfLines* collectResultsFromDistribution(int numOfProcess, char* tempDir);
void printListOfLines(ListOfLines* list, int numOfLists);
#endif