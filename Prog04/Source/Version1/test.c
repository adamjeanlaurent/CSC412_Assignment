#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fileSystemHandler.h"
#include "processing.h"
#include "distributor.h"

typedef struct Line
{
   int index;
   int lineNum;
   char* contents;
} Line;

typedef struct ListOfLines
{
    int length;
    Line* lines;
} ListOfLines;

ListOfLines* test()
{
    int numOfProcess = 3;
    ListOfLines* listOfLines = (malloc(sizeof(ListOfLines) * 2));

    int i;

    // Line 0:1
    char* line = "printf(hello world)";
    int index = 5;
    int lineNum = 10;

    listOfLines[0].length = 1;
    listOfLines[0].lines = (Line*)malloc(sizeof(Line));
    listOfLines[0].lines[0].contents = (char*)calloc(strlen(line) + 1, sizeof(char));
    sprintf(listOfLines[0].lines[0].contents, "%s", line);
    listOfLines[0].lines[0].index = index;
    listOfLines[0].lines[0].lineNum = lineNum;

     // Line 0:1
    char* line2 = "printf(goodbye world)";
    int index2 = 53;
    int lineNum2 = 189;

    listOfLines[1].length = 10;
    listOfLines[1].lines = (Line*)malloc(sizeof(Line));
    listOfLines[1].lines[0].contents = (char*)calloc(strlen(line2) + 1, sizeof(char));
    sprintf(listOfLines[1].lines[0].contents, "%s", line2);
    listOfLines[1].lines[0].index = index2;
    listOfLines[1].lines[0].lineNum = lineNum2;

    return listOfLines;
}

int main(int argc, char** argv)
{
    // char* f = "../../Data Sets/Data Set 1/";
    // int i;
    
   //  FILE* fp = fopen("./temp/toDoList_1.txt","r");

   
// ListOfLines* t =  test();

// printf("\n length: %d \n Lines: \n line[0] index: %d \n line[0] lineNum: %d \n line[0] contents: %s \n", 
// t[0].length, 
// t[0].lines[0].index, 
// t[0].lines[0].lineNum,
// t[0].lines[0].contents
// );


// printf("\n length: %d \n Lines: \n line[1] index: %d \n line[1] lineNum: %d \n line[1] contents: %s \n", 
// t[1].length, 
// t[1].lines[0].index, 
// t[1].lines[0].lineNum,
// t[1].lines[0].contents
// );























    // if(fp != NULL)
    // {
    //     char buffer[500];
    //     while(fscanf(fp,  " %[^\n]", buffer) == 1)
    //     {
    //         puts(buffer);
    //     }
    //     fclose(fp);
    // }

    // Array2D* a = getFileList(f);
    
    // int numOfFiles = a->rows;
    
    // for(i = 0; i < numOfFiles; i++)
    // {
    //     FILE* fp = fopen(a->contents[i] ,"r");
    //     int index;
    //     int lineNum;
    //     char* buffer = (char*)calloc(1000, sizeof(char));

    //     if(fp != NULL)
    //     {
    //         // fscanf(fp, "%d%d%[^\n]s", &index, &lineNum, buffer);
    //         fscanf(fp, "%d %d", &index, &lineNum);
    //         fgets(buffer, 1000, fp);

    //         printf("------------ \nfilename: %s\nindex: %d\nline number: %d\nbuffer: %s\n------------", a->contents[i], index, lineNum, buffer);
            // if(strlen(buffer) == 1)
            // {
            //     puts(a->contents[i]);
        //     // }
        // }
    // }
}