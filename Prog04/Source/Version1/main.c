#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fileSystemHandler.h"
#include "processing.h"

int main(int argc, char** argv)
{
    // if(argc < 3)
    // {
    //     printf("usage: %s numOfProcesses DataFolderPath OutputPath \n", argv[0]);
    //     return 0;
    // }

    // char* filePath = "../../Data Sets/Data Set 1/";
    char* f = "/Users/ajean-laurent/Dropbox/CSC_412_Work/Prog04/Data Sets/Data Set 1";

    Array2D* a = getFileList(f);

    int numOfFiles = a->rows;
    int numOfProcesses = 3;

    Array2D** workLists = splitWork(numOfFiles, numOfProcesses, a);

    int i;
    for(i = 0; i < numOfProcesses; i++)
    {
        printf("Work List %d: \n", i);
        printArray2D(workLists[i]);
    }

    for(i = 0; i < numOfProcesses; i++)
    {
       freeArray2D(workLists[i]);
    }

    free(workLists);
    freeArray2D(a);
    return 0;
}