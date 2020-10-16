#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fileSystemHandler.h"

int main(int argc, char** argv)
{
    char* filePath = "../../Data Sets/Data Set 1/";
    // if(argc < 3)
    // {
    //     printf("usage: %s numOfProcesses DataFolderPath OutputPath \n", argv[0]);
    //     return 0;
    // }

    Array2D* a = getFileList(filePath);
    printArray2D(a);
    return 0;
}