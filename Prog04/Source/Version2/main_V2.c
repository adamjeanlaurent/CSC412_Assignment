// system includes
#include <stdio.h> 
#include <stdlib.h>

// POSIX includes
#include <sys/types.h> 
#include <unistd.h>
#include <sys/wait.h>

// custom includes
#include "../fileSystemHandler.h"
#include "../processing.h"
#include "../distributor.h"
#include "../splitWork.h" 

int main(int argc, char** argv)
{
    // verify correct number of arguments
    if(argc != 4)
    {
        printf("usage: %s numOfProcesses DataFolderPath OutputPath \n", argv[0]);
        return 0;
    }
    
    // parse arguments
    int numOfProcesses = atoi(argv[1]) + 1;
    char* dataSetFilePath = argv[2];
    char* outputFilePath = argv[3];
    char* tempDir = "./temp/";

    int i;

    // get list of all files in the data set directory
    Array2D* dataFileList = getFileList(dataSetFilePath);

    int numOfFiles = dataFileList->rows;

    // split data files evenly into n lists
    Array2D** workLists = splitWork(numOfFiles, numOfProcesses, dataFileList);

    // holds process ids of distribution children processes
    pid_t distrubutorChildProcessTable[numOfProcesses];

    for(i = 0; i < numOfProcesses; i++)
    {
        // create new process
        pid_t pid = fork();

        // if child process distribute files
        if(pid == 0)
        {   
            distribute(workLists[i], i, tempDir);
            exit(0);
        }
        // if not a child process add to process table
        else
        {
            distrubutorChildProcessTable[i] = pid;
        }
    }

    int status = 0;

    // wait for distribution children to finish
    for(i = 0; i < numOfProcesses; i++)
    {
        waitpid(distrubutorChildProcessTable[i], &status, 0);
    }

    // store results of distribution into Array of ListOfLines for easier sorting
    ListOfLines* distributedDataFiles = collectResultsFromDistribution(numOfProcesses, tempDir);

    // holds process ids of data processing children processes
    pid_t dataProcessingChildProcessTable[numOfProcesses];

    for(i = 0; i < numOfProcesses; i++)
    {
        // create new process
        pid_t pid = fork();

        // if child process data files
        if(pid == 0)
        {
            processV2(&distributedDataFiles[i], tempDir, i);
            exit(0);
        }
        // if not a child process ad to process table
        else
        {
            dataProcessingChildProcessTable[i] = pid;
        }
    }
    
    // wait for data processing children to finish
    for(i = 0; i < numOfProcesses; i++)
    {
        waitpid(dataProcessingChildProcessTable[i], &status, 0);
    }

    // combine sorted fragments into full source file
    concatSourceFragments(outputFilePath, numOfProcesses, tempDir);
    
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