#include <stdio.h> 
#include <stdlib.h>
#include <sys/types.h> 
#include <unistd.h>
#include <sys/wait.h>
#include "../fileSystemHandler.h"
#include "../processing.h"
#include "../distributor.h"
#include "../splitWork.h" 

int main(int argc, char** argv)
{
     if(argc != 4)
    {
        printf("usage: %s numOfProcesses DataFolderPath OutputPath \n", argv[0]);
        return 0;
    }
    
    int numOfProcesses = atoi(argv[1]) + 1;
    char* dataSetFilePath = argv[2];
    char* outputFilePath = argv[3];
    char* tempDir = "../Source/Version2/temp/";

    int i;

    Array2D* dataFileList = getFileList(dataSetFilePath);

    int numOfFiles = dataFileList->rows;

    Array2D** workLists = splitWork(numOfFiles, numOfProcesses, dataFileList);

    pid_t distrubutorChildProcessTable[numOfProcesses];

    for(i = 0; i < numOfProcesses; i++)
    {
        pid_t pid = fork();

        if(pid == 0)
        {   
            distribute(workLists[i], i, tempDir);
            exit(0);
        }
        else
        {
            distrubutorChildProcessTable[i] = pid;
        }
    }

    int status = 0;
    for(i = 0; i < numOfProcesses; i++)
    {
        waitpid(distrubutorChildProcessTable[i], &status, 0);
    }

    ListOfLines* distributedDataFiles = collectResultsFromDistribution(numOfProcesses, tempDir);

    pid_t dataProcessingChildProcessTable[numOfProcesses];

    for(i = 0; i < numOfProcesses; i++)
    {
        pid_t pid = fork();

        if(pid == 0)
        {
            processV2(&distributedDataFiles[i], tempDir, i);
            exit(0);
        }
        else
        {
            dataProcessingChildProcessTable[i] = pid;
        }
        
    }

    for(i = 0; i < numOfProcesses; i++)
    {
        waitpid(dataProcessingChildProcessTable[i], &status, 0);
    }

    concatSourceFragments(outputFilePath, numOfProcesses, tempDir);

    // plan:    

    // Step 1: get file list
    // Step 2: split work
    // Step 3: Create n child processes, store pids in array, assign each process an index # (i)
    // Step 4: each child process calls distribute with it's index of the work list
    // Step 5: Parent process waits for these children to finish
    // Step 6: collect results, translate to new data structure in main
    // Step 7: create n more children to do the data processing
    // Step 8: each data process call writes it's output to a file
    // Step 9: wait for data processeses to finsih
    // Step 10: main reads n files and concats them into the output file

    // new functionality needed:

    // getting file list is same
    // splitting work is same
    // distribute is same
    // collect results is same
    // data processing needs to write to a file instead of returning a long string (what should be write to this file? sorted file names or lines?)
    // read n sorted files and output to one source file 

    return 0;
}