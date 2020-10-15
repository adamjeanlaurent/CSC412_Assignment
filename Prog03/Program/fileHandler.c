#include "fileHandler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "matrix.h"

// takes in a path to a file, for the sake of this program, .pat and .img
// converts file contents in 2DArray, stored in char** Matrix.values
Matrix* fileDataTo2DArray(const char *filePath)
{
    // open file from specified path
    FILE *fp;
    fp = fopen(filePath, "r");
    // exit if fopen resulted in NULL
    if(fp == NULL)
    {
        exit(1);
    }
    
    // allocate memory for matrix
    Matrix *m = (Matrix *)malloc(sizeof(Matrix));
    m->values = NULL;

    // read first line of file which captures the number of rows and number of columns to read
    fscanf(fp, "%d%d ", &m->columns, &m->rows);

    // allocate memory for number of rows in matrix
    m->values = (char**)malloc(sizeof(char*) * m->rows);

    int i;
    int j;

    // allocated memory for number of columns in each row in matrix
    for (i = 0; i < m->rows; i++)
    {
        m->values[i] = (char*)malloc(sizeof(char) * (m->columns + 1));
        m->values[i][m->columns] = '\0';
    }

    // read entire first, character by character, string each character in it's place in the matrix
    for (i = 0; i < m->rows; i++)
    {
        for (j = 0; j < m->columns; j++)
        {
            fscanf(fp, "%c ", &m->values[i][j]);
        }
    }
    // close file
    fclose(fp);
    
    return m;
}

// prints matrix
// for debugging purposes
void printMatrix(Matrix *m) 
{
    int i;
    int j;
    for (i = 0; i < m->rows; i++)
    {
        for (j = 0; j < m->columns; j++)
        {
            printf("%c", m->values[i][j]);
        }
        printf("\n");
    }
}

//  custom destructor for properly freeing a matrix
// frees all rows then frees Matrix struct pointer
void freeMatrix(Matrix *m)
{
    int i;

    for(i = 0; i < m->rows; i++)
    {
        // free row
        if(m->values[i] != NULL)
        {
            free(m->values[i]);
        }
        
        m->values[i] = NULL;
    }

    if(m->values != NULL)
    {
        free(m->values);
    }

    m->values = NULL;

    if(m != NULL)
    {
        free(m);    
    }

    m = NULL;
}

// writes pattern match info to an output file
// writes a single set of patterns matches from one image and pattern pair at a time
// example:
// image1.img
//      4 2 3 4 5 6 3 2 1
void writePatternMatchesToOutputFile(char* patternFilePath, char* imageFilename, PatternMatchInfo* patternMatches, char* outputDirectory)
{
    
    // open file in r+ mode so that we can know if the file exists already
    // r+ mode returns NULL if file doesn't exist
    FILE *fp = fopen(outputDirectory, "r+");
    int fileExists = 1; // TRUE 
    int i;

    if(fp == NULL)
    {
        fileExists = 0; // FALSE
    }
    
    else
    {
        fclose(fp);
    }
    
    // open file in a mode
    // a mode will create file if it doesn't exist already
    fp = fopen(outputDirectory, "a");
    
    if(fp != NULL)
    {
        // if the file existed already, write a newline to the file so that we don't write pattern matches for different images on the same line
        // if the file didn't exist already, don't write a newline so that the file doesn't start with a newline
        if(fileExists)
        {
            fputs("\n", fp);
        }

        // write image file name to file
        fputs(imageFilename, fp);

        // write new line to file
        fputs("\n", fp);

        // write tab to file
        fputs("\t ", fp);

        // write the number of times the pattern was found in the image to the file
        fprintf(fp, "%d ", patternMatches->numberOfTimesFound);
        
        // print all pattern matches with their row and column locations
        for(i = 0; i < patternMatches->size; i++)
        {
            fprintf(fp, "%d ", patternMatches->placesFound[i]);
        }
        fclose(fp);
    }
}