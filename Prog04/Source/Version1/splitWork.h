#ifndef SPLITWORK_H
#define SPLITWORK_H

#include "fileSystemHandler.h"

int* getIndexRangesForSplittingWork(int numOfFiles, int numOfProcess);
Array2D** splitWork(int numOfFiles, int numOfProcess, Array2D* fileList);

#endif