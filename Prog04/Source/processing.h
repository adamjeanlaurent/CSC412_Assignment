#ifndef PROCESSING_V1_H
#define PROCESSING_V1_H

#include "fileSystemHandler.h"

/** Represents a single line of a source file. */
typedef struct Line
{
    /** index of line. */
    int index;
    /** line number of line. */
    int lineNum;
    /** string storing source code for the line. */
    char* contents;
} Line;

/** Stores an array of struct Line, along with the number of elements in the array. */
typedef struct ListOfLines
{
    /** length of lines array. */
    int length;
    /** array of struct Line. */
    Line* lines;
} ListOfLines;

/**
 * Summary: Used after distribution step, reads to-do lists text files and stores them into an array of ListOfLines, where each ListOfLines represents a list of lines in a to-do list. 
 * @param numOfProcess: the number of child processes created in the dispatcher.
 * @param tempDir: relative path to directory that holds temporary files for inter-process communcation.
 * @return pointer to array of ListOfLines where each ListOfLines holds a to-do list for data processing.
 */ 
ListOfLines* collectResultsFromDistribution(int numOfProcess, char* tempDir);

/**
 * Summary: prints an array of ListOfLines structs for debugging purposes.
 * @param listOfLines: pointer to listOfLines struct array to print out.
 * @param numOfLists: length of listOfLines array.
 * @return void.
 */ 
void printListOfLines(ListOfLines* listsOfLines, int numOfLists);

/**
 * Summary: Performs data processing for version 1. Sorts an array of ListOfLines->lines by indexNumber, then builds a long string representing an in order fragment of a source file.
 * @param listOfLines: pointer to a list of lines to sort and build into string.
 * @return long string containing entire sorted fragment of source file.
 */ 
char* processV1(ListOfLines* listOfLines);

/**
 * Summary: Sorting function to pass the qsort when sorting ListOfLines->lines, sorts by index number.
 * @param a: pointer to first item .
 * @param b: pointer to second item.
 * @return a->indexNumber minus b->indexNumber.
 */ 
int sortingByAscendingFunction(const void* a, const void* b);

/**
 * Summary: Performs data processing for version1. sorts an array of ListOfLines->lines by indexNumber, then writes sorted lines of fragment to fragment file.
 * @param listOfLines: pointer to a list of lines to sort and write to file.
 * @param tempDir: relative path to directory that holds temporary files for inter-process communcation.
 * @param processNum: index of data processing process that will process this fragment.
 * @return void, writes output to fragment file.
 */ 
void processV2(ListOfLines* listOfLines, char* tempDir, int processNum);

/**
 * Summary: generates file name to store fragment file after processing in version 2. example: ./tempDir/Fragment_processNum/
 * @param tempDir: relative path to directory that holds temporary files for inter-process communcation.
 * @param processNum: index of data processing process that processed this fragment.
 * @return string containing file path created.
 */ 
char* generateSourceFragmentFileName(char* tempDir, int processNum);

/**
 * Summary: Custom destructor for a list of lines array.
 * @param listOfLines: pointer to an array of malloced list of line objects.
 * @param numOfLists: number of elements in the array.
 * @return void
 */ 
void freeListOfLines(ListOfLines* listOfLines, int numOfLists);

#endif