#ifndef DISTRIBUTOR_H
#define DISTRIBUTOR_H

#include "fileSystemHandler.h"

/**
 * Summary: The purpose of this function is to look a list of files, read each file, determine what process is resposible for it by reading it's index, 
 * and writing the files path to the processes to do list. 
 * @param workList: list of files paths that this process is responsible to distribute.
 * @param currentProcess: the distributer number, example if d7, currentProcess = 7.
 * @param tempDir: relative path to directory that holds temporary files for inter-process communcation.
 * @return void
 */
void distribute(Array2D* worklist, int currentProcess, char* tempDir);

/**
 * Summary: Generates the to-do list filename of a distributor. The format is combining the process #, tempDir path, and _toDoList. example. ./temp/toDoList_4.txt .
 * @param tempDir: relative path to directory that holds temporary files for inter-process communcation.
 * @param processNum: the distributer number, example if d7, processNum = 7.
 * @return to-do list filename
 */
char* generateToDoListFileName(char* tempDir, int processNum);

/**
 * Summary: Computes the digit length of a given integer.
 * @param num: integer to get the digit length of.
 * @return digit length of given integer.
 */
int fastLengthOfNumber(int num);

#endif