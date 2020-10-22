// system includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// custom includes
#include "../fileSystemHandler.h"
#include "../processing.h"
#include "../distributor.h"
#include "../splitWork.h"

int main(int argc, char** argv)
{
    // verify the corrent number of arguments were passed
    if(argc != 4)
    {
        printf("usage: %s numOfProcesses DataFolderPath OutputPath \n", argv[0]);
        return 0;
    }
    
    // parse input arguments
    int numOfProcesses = atoi(argv[1]) + 1; // + 1 because processes are 0 indexed
    char* dataSetFilePath = argv[2];
    char* outputFilePath = argv[3];
    char* tempDir = "../Source/Version1/temp/";

    int i;

    // get list of all files in data directory
    Array2D* dataFileList = getFileList(dataSetFilePath);

    int numOfFiles = dataFileList->rows;

    // split data files evenly into n lists
    Array2D** workLists = splitWork(numOfFiles, numOfProcesses, dataFileList);

    // distribute fragments into to-do lists based on index
    for(i = 0; i < numOfProcesses; i++)
    {
        distribute(workLists[i], i, tempDir);
    }
    
    // store results from distribution step for easier sorting in data processing step
    ListOfLines* distributedDataFiles = collectResultsFromDistribution(numOfProcesses, tempDir);

    char* reconstructedSourceFile = (char*)calloc(1, sizeof(char));
    reconstructedSourceFile[0] = '\0';

    // process distribute fragments
    for(i = 0; i < numOfProcesses; i++)
    {
        // store entire sorted fragment of source file in string
        char* partialOfReconstructedSourceFile = processV1(&distributedDataFiles[i]);

        // compute new length of entire source file
        int len = strlen(partialOfReconstructedSourceFile) + strlen(reconstructedSourceFile) + 1;

        // allocate more space in output file for processed fragment
        reconstructedSourceFile = (char*)realloc(reconstructedSourceFile, sizeof(char) * len);

        // copy processed fragment into output string
        strcat(reconstructedSourceFile, partialOfReconstructedSourceFile);
        free(partialOfReconstructedSourceFile);
    }
    
    // write string containing output file to actual output file
    writeStringToFile(reconstructedSourceFile, outputFilePath);
    
    // free all dispatcher dynamically allocated memory 

    // free work lists
    for(i = 0; i < numOfProcesses; i++)
    {
        freeArray2D(workLists[i]);
    }
    
    // free work list
    free(workLists);
    
    // free data list file
    freeArray2D(dataFileList);

    // free list of distributed data files
    freeListOfLines(distributedDataFiles, numOfProcesses);
    return 0;
}