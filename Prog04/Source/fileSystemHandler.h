#ifndef FILESYSTEMHANDLER_V1_H
#define FILESYSTEMHANDLER_V1_H

/** Stores a 2D array of characters. In laymans terms, an array of string. */
typedef struct Array2D {
    /**  */
    int rows;
    char** contents;
} Array2D;

/**
 * Summary: Gives list of all files in the given directory.
 * @param filePath: file path of directory to get all files from.
 * @return Array2D* stroing file paths of all files in given directory.
 */ 
Array2D* getFileList(char* filePath);

/**
 * Summary: concatinates a file name and file path ex. fullPath/fileName.extension . 
 * @param fileName: file name to concat.
 * @param filePath: file path to concat.
 * @param lengthOfFileName: character length of fileName param.
 * @param lengthOfFilePath: character length of FilePath param.
 * @return string storing concatinated file name and path.
 */ 
char* concatFileNameAndPath(char* fileName, char* filePath, int lengthOfFileName, int lengthOfFilePath);

/**
 * Summary: Prints an Array2D's contents, for debugging purposes.
 * @param array2D: pointer to Array2D to print the contents of.
 * @return void
 */ 
void printArray2D(Array2D* array2D);

/**
 * Summary: Custom destructor for an Array2D struct.
 * @param array2D: pointer to Array2D struct to free.
 * @return void
 */ 
void freeArray2D(Array2D* array2D);

/**
 * Summary: Writes a string to a file.
 * @param reconstructedFile: string to write to file, called reconstructedFile because it's used in Version 1 to write a long string representing a fragment of a source file to an output source file.
 * @return void
 */ 
void writeStringToFile(char* reconstructedFile, char* outputPath);

/**
 * Summary: Reads a number of files containing fragments of a source file and concatinates them into one output file in the correct order.
 * @param outputPath: output file path to destination source file.
 * @param numOfProcess: the number of processes used in the program, determines the number of fragments to concatinate.
 * @param tempDir: relative path to directory that holds temporary files for inter-process communcation.
 * @return
 */ 
void concatSourceFragments(char* outputPath, int numOfProcess, char* tempDir);

#endif