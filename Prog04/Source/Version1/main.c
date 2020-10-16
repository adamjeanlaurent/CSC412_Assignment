#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fileSystemHandler.h"
#include "processing.h"

int main(int argc, char** argv)
{
    // char* filePath = "../../Data Sets/Data Set 1/";
    // if(argc < 3)
    // {
    //     printf("usage: %s numOfProcesses DataFolderPath OutputPath \n", argv[0]);
    //     return 0;
    // }

    // Array2D* a = getFileList(filePath);

    // int numOfFiles = a->rows;

    // freeArray2D(a);

    splitWork(50, 20);
    splitWork(105, 20);


    return 0;
}