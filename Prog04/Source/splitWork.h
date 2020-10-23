#ifndef SPLITWORK_V1_H
#define SPLITWORK_V1_H

#include "fileSystemHandler.h"

/**
 * Summary: Splits a larger number (numOfFiles) into n (numOfProcess) parts, keeping the difference between the n parts minimal.
 * @param numOfFiles: number of file to split.
 * @param numOfProcess: number of processes that the files will be broken into.
 * @return array of integers storing the split parts. Example: numOfFiles = 40, numOfProcess = 15. output = [2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3]
 */ 
int* getIndexRangesForSplittingWork(int numOfFiles, int numOfProcess);

/**
 * Summary: splits work of processes into as equal an amount as possible for distribution. 
 * @param numOfFiles: number of files in total.
 * @param numOfProcess: number of process that will process these files.
 * @param fileList: list of files.
 * @return array of Array2D structs where each Array2D struct stores file names process x will distribute.
 */ 
Array2D** splitWork(int numOfFiles, int numOfProcess, Array2D* fileList);

#endif