#ifndef PROCESSING_H
#define PROCESSING_H

#include "fileSystemHandler.h"

typedef struct Array3D {
    int numOfLists;
    Array2D** lists;
} Array3D;

int* getIndexRangesForProcesses(int numOfFiles, int numOfProcess);
Array2D** splitWork(int numOfFiles, int numOfProcess, Array2D* fileList);

#endif