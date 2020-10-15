#ifndef PATTERNMATCHING_H
#define PATTERNMATCHING_H
#include "matrix.h"
#include "patternMatchOutput.h"

int indexOf(char* stringToSearch, char* stringToFind);
void getExtension(const char* fileName, char* extension);
int isDotImgFile(const char* fileName);
void getPatternMatches(const char* imgDirectory, Matrix* pattern, char* patternDirectory, char* outputDirectory);
void getAllPatternMatchesFromImgFile(const char* filePath, Matrix* pattern, PatternMatchInfo* output);
#endif
