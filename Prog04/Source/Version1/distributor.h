#ifndef DISTRIBUTOR_H
#define DISTRIBUTOR_H

#include "fileSystemHandler.h"

// functionality needed

// takes in Array2D List of files
// reads file, writes file name to it's corresponding to do list file

void distribute(Array2D* worklist, int currentProcess, char* tempDir);
char* generateToDoListFileName(char* tempDir, int processNum);
int fastLengthOfNumber(int num);

#endif