#ifndef FILESYSTEMHANDLER_H
#define FILESYSTEMHANDLER_H

typedef struct Array2D {
    int rows;
    char** contents;
}Array2D;

Array2D* getFileList(char* filePath);
char* concatFileNameAndPath(char* fileName, char* filePath, int lengthOfFileName, int lengthOfFilePath);
void printArray2D(Array2D* array2D);
void freeArray2D(Array2D* array2D);

#endif