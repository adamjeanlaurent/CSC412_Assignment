#include <stdio.h>
#include <stdlib.h>
#include "fileHandler.h"
#include "patternMatching.h"

int main(int argc, char** argv) 
{
    // capture program arguments
    char* patternFilePath = argv[1];
    char* imagesFilePath = argv[2];
    char* outputFilePath = argv[3];

    // capture pattern into Matrix for easier comparison
    Matrix* pattern = fileDataTo2DArray(patternFilePath);

    // compare images and pattern to find matches and write to output path
    getPatternMatches(imagesFilePath, pattern, patternFilePath, outputFilePath);
    
    if(pattern != NULL) 
    {
    	freeMatrix(pattern);
    }
}