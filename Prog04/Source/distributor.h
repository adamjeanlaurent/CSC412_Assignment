#ifndef DISTRIBUTOR_H
#define DISTRIBUTOR_H

#include "fileSystemHandler.h"

void distribute(Array2D* worklist, int currentProcess, char* tempDir);
char* generateToDoListFileName(char* tempDir, int processNum);
int fastLengthOfNumber(int num);

#endif