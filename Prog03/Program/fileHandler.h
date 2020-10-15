#ifndef FILEHANDLER_H
#define FILEHANDLER_H
#include "matrix.h"
#include "patternMatchOutput.h"

Matrix* fileDataTo2DArray(const char* filePath);
void printMatrix(Matrix *m);
void freeMatrix(Matrix* m);
void writePatternMatchesToOutputFile(char* patternFilePath, char* imageFilename, PatternMatchInfo* patternMatches, char* outputDirectory);

#endif
