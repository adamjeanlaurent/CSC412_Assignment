#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fileSystemHandler.h"
#include "processing.h"
#include "distributor.h"
#include "splitWork.h"

int main(int argc, char** argv)
{
    // if(argc < 3)
    // {
    //     printf("usage: %s numOfProcesses DataFolderPath OutputPath \n", argv[0]);
    //     return 0;
    // }
    
    char* f = "../../Data-Sets/Data-Set-1/";
    char* tempDir = "./temp/";
    int numOfProcesses = 2;
    int i;

    Array2D* a = getFileList(f);

    int numOfFiles = a->rows;

    Array2D** workLists = splitWork(numOfFiles, numOfProcesses, a);

    for(i = 0; i < numOfProcesses; i++)
    {
        distribute(workLists[i], i, tempDir);
    }
    
    ListOfLines* l = collectResultsFromDistribution(numOfProcesses, tempDir);
    // printListOfLines(l, numOfProcesses);


    char* reconstructedSourceFile = (char*)calloc(1, sizeof(char));
    reconstructedSourceFile[0] = '\0';

    for(i = 0; i < numOfProcesses; i++)
    {
        char* partialOfReconstructedSourceFile = process(&l[i]);
        int len = strlen(partialOfReconstructedSourceFile) + strlen(reconstructedSourceFile) + 1;
        reconstructedSourceFile = (char*)realloc(reconstructedSourceFile, sizeof(char) * len);
        strcat(reconstructedSourceFile, partialOfReconstructedSourceFile);
        free(partialOfReconstructedSourceFile);
    }
      puts(reconstructedSourceFile);
   
    
    for(i = 0; i < numOfProcesses; i++)
    {
        freeArray2D(workLists[i]);
    }
    
    free(workLists);
    freeArray2D(a);
    return 0;
}