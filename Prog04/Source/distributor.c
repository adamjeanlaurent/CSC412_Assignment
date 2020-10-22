// system includes
#include <stdio.h>
#include <stdlib.h>

// custom includes
#include "distributor.h"
#include "string.h"

void distribute(Array2D* workList, int currentProcess, char* tempDir)
{
    int i;

    // iterate through every file path in the work list
    for(i = 0; i < workList->rows; i++)
    {
        char* filePath = workList->contents[i];
        FILE* fp;
        int index;

        // open file path of fragment file
        fp = fopen(filePath, "r");
    
        if(fp != NULL)
        {
            // get index number fragment file
            fscanf(fp, "%d", &index);
            fclose(fp);
            
            // generate to do list file name of index found
            char* toDoListFilePath = generateToDoListFileName(tempDir, index); 

            // open / create to do list file 
            fp = fopen(toDoListFilePath, "a");

            if(fp != NULL)
            {
                // write fragment file path to do list
                fprintf(fp, "%s\n", filePath);
                fclose(fp);
            }
    
            free(toDoListFilePath);
        }
    }
}

char* generateToDoListFileName(char* tempDir, int processNum)
{
    // get length of processNum
    int lengthOfProcessNum = fastLengthOfNumber(processNum);
    char* toDoListFileName = "toDoList_";

    // allocate memory for full path + file name + extension + number 
    // combine into string
    int len = strlen(tempDir) + lengthOfProcessNum + strlen(toDoListFileName) + 5;
    char* toDoListFilePath = (char*)calloc(len, sizeof(char));
    sprintf(toDoListFilePath, "%s%s%d.txt", tempDir, toDoListFileName, processNum);
    
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