// system includes
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// custon includes
#include "fileSystemHandler.h"
#include "processing.h"

// POSIX includes
#include <sys/types.h>
#include <dirent.h>
#include <libgen.h>

Array2D* getFileList(char *filePath)
{
    Array2D* fileList = (Array2D*)malloc(sizeof(Array2D));
    fileList->contents = (char**)malloc(sizeof(char*));

    int lengthOfFilePath = strlen(filePath);

    fileList->rows = 0;
    
    DIR* dp;
    struct dirent* ep;
    dp = opendir(filePath);

    if(dp != NULL)
    {
        while((ep = readdir(dp)) != NULL)
        {
            if(ep->d_type == DT_REG)
            {
                char* fileName = ep->d_name;
                int lengthOfFileName = strlen(fileName);

                //combine file name and path
                char* combinedPath = concatFileNameAndPath(fileName, filePath, lengthOfFileName, lengthOfFilePath);

                int row = fileList->rows;

                // allocate chars in row
                fileList->contents[row] = (char*)calloc((lengthOfFileName + lengthOfFilePath + 1), sizeof(char));

                // copy full path to newly allocated row
                sprintf(fileList->contents[row], "%s", combinedPath);
                fileList->rows++;
                
                // increase number of rows by 1
                fileList->contents = (char**)realloc(fileList->contents, sizeof(char*) * (fileList->rows + 1));

                // free full path
                free(combinedPath);
            }
        }
    }
    return fileList;
}

char* concatFileNameAndPath(char* fileName, char* filePath, int lengthOfFileName, int lengthOfFilePath)
{
    char* combinedPath = (char*)calloc((lengthOfFileName + lengthOfFilePath + 1), sizeof(char));
    sprintf(combinedPath, "%s%s", filePath, fileName);
    
    return combinedPath;
}

void printArray2D(Array2D* array2D) 
{
    int i;

    for(i = 0; i < array2D->rows; i++)
    {
        puts(array2D->contents[i]);
    }
}

void freeArray2D(Array2D* array2D)
{
    int i;

    for(i = 0; i < array2D->rows; i++)
    {
        if(array2D->contents[i] != NULL)
        {
             free(array2D->contents[i]);
        }
       
    }
    
    if(array2D->contents != NULL)
    {
          free(array2D->contents);
    }
  
    if(array2D != NULL)
    {
        free(array2D);
    }
}

void writeStringToFile(char* reconstructedFile, char* outputPath)
{
    FILE* fp = fopen(outputPath ,"a");
    
    if(fp != NULL)
    {
         fputs(reconstructedFile, fp);
         fclose(fp);
    }
}

void concatSourceFragments(char* outputPath, int numOfProcess, char* tempDir)
{
    int i;  
    int j;

    // create output file
    FILE* outputFP = fopen(outputPath ,"a");

    if(outputFP != NULL)
    {
        // read source fragments
        for(i = 0; i < numOfProcess; i++)
        {
            char* fragmentFilePath = generateSourceFragmentFileName(tempDir, i);
            
            // open fragment
            FILE* fragmentFP = fopen(fragmentFilePath ,"r");

            if(fragmentFP != NULL)
            {
                // read fragment file
                int numOfLinesInFile = fscanf(fragmentFP, "%d", &numOfLinesInFile);

                char buffer[500];
                // read all lines in file and write to output
                for(j = 0; j < numOfLinesInFile; j++)
                {       
                    fgets(buffer, 500, fragmentFP);

                    // remove newline from buffer
                    buffer[strcspn(buffer, "\n")] = 0;

                    // write buffer to output file
                    fprintf(outputFP, "%s\n", buffer);
                }   
                fclose(fragmentFP);
            }
        }
        fclose(outputFP);
    }
}