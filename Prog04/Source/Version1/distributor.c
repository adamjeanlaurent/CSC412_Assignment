#include "distributor.h"
#include <stdio.h>
#include <stdlib.h>
#include "string.h"

void distribute(Array2D* workList, int currentProcess, char* tempDir)
{
    int i;

    for(i = 0; i < workList->rows; i++)
    {
        char* filePath = workList->contents[i];
        FILE* fp;
        int index;

        fp = fopen(filePath, "r");

        if(fp != NULL)
        {
            // get index number of file
            fscanf(fp, "%d", &index);
            fclose(fp);

            char* toDoListFilePath = generateToDoListFileName(tempDir, currentProcess);

            fp = fopen(toDoListFilePath, "a");
            fprintf(fp, "%s\n", filePath);

            free(toDoListFilePath);
        }

        // write to corresponding toDoListFile
        // assuming dir already exists
        // and that bash script will delete it later

        // fp = fopen(tempDir, "a");
        
        // // output file name to file
        // fprintf(fp, "%s\n", );
        
    }
}

char* generateToDoListFileName(char* tempDir, int processNum)
{
    int lengthOfProcessNum = fastLengthOfNumber(processNum);
    char* toDoListFileName = "toDoList_";

    int len = strlen(tempDir) + lengthOfProcessNum + strlen(toDoListFileName) + 1;
    char* toDoListFilePath = (char*)calloc(len, sizeof(char));
    sprintf(toDoListFilePath, "%s%s%d", tempDir, toDoListFileName, processNum);
    
    return toDoListFilePath;
}

int fastLengthOfNumber(int num) {
    if (num >= 1000000000) return 10;
    if (num >= 100000000)  return 9;
    if (num >= 10000000)   return 8;
    if (num >= 1000000)    return 7;
    if (num >= 100000)     return 6;
    if (num >= 10000)      return 5;
    if (num >= 1000)       return 4;
    if (num >= 100)        return 3;
    if (num >= 10)         return 2;
    return 1;
}