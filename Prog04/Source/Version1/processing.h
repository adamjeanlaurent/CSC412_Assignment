#ifndef PROCESSING_H
#define PROCESSING_H

#include "fileSystemHandler.h"

typedef struct Array3D {
    int rows;
    Array2D* contents;
}Array3D;

void splitWork(int numOfFiles, int numOfProcess);

#endif