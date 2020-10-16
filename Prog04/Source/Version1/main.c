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

    char* filePath = "../../Data Sets/Data Set 1/";

    Array2D* a = getFileList(filePath);

    int numOfFiles = a->rows;
    int numOfProcesses = 3;

    Array2D** workLists = splitWork(numOfFiles, numOfProcesses, a);

    int i;
    for(int i = 0; i < numOfProcesses; i++)
    {
        printf("Work List %d: \n");
        printArray2D(workLists[i]);
    }


    for(int i = 0; i < numOfProcesses; i++)
    {
       freeArray2D(workLists[i]);
    }


    freeArray2D(a);
    return 0;
}