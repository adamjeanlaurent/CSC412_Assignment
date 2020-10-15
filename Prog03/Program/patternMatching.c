#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "patternMatching.h"
#include "fileHandler.h"

// POSIX includes
#include <sys/types.h>
#include <dirent.h>
#include <libgen.h>

// helper method to find index of pointer returned from strstr
// example:
// s1 = "hello"
// pointer = strstr(s1, "lo");
// indexOf(pointer) => 3 
int indexOf(char* stringToSearch, char* stringToFind)
{
	int index = -1;
	
	index = (stringToFind - stringToSearch);
	
	return index;
}

// get the extension of a file and write to extension string
void get_extension(const char* fileName, char* extension)
{
	char* dot = strchr(fileName, '.');

	//no file extension case
	if (dot == NULL) 
	{
		// if extension not found, make extension equal to a blank string
		extension[0] = '\0'; 
	}
	// return the string found after the last . in the filename
	else
	{
		char* tmp = dot;
		while (tmp != NULL)
		{
			tmp = strchr(tmp + 1, '.');
			if (tmp != NULL)
				dot = tmp;
		}
		strcpy(extension, dot);
	}
}

// returns 1 if file extension is .img
// returns 0 otherwise
int isDotImgFile(const char* fileName)
{
	char extension[100];

	get_extension(fileName, extension);

	if(strcmp(extension, ".img") == 0) 
	{
		return 1;
	}
	
	return 0;
}

// this function takes care of looping through the image directory all calling helper function to find pattern matches and write them to files
void getPatternMatches(const char* imgDirectory, Matrix* pattern, char* patternDirectory, char* outputDirectory)
{
	// allocate memory and initialize PatternMatchInfo struct which holds information about an image and patterns matches
	PatternMatchInfo* patternMatches = (PatternMatchInfo*)malloc(sizeof(PatternMatchInfo));
	patternMatches->numberOfTimesFound = 0;
	patternMatches->size = 0;

	DIR *dp;
	struct dirent *ep;
	dp = opendir(imgDirectory);
		
	// get full outputfile name
	char* patternFileName = basename(patternDirectory);

	// combine output file with with pattern file name and _matches.txt
	char* underScoreMatchesTxt = "_Matches.txt";
	int len = strlen(patternFileName) - 3; // remove extension from pattern file name
	patternFileName[len - 1] = '\0';

	char* fullOutputPath = (char*)malloc( (len + strlen(underScoreMatchesTxt) + strlen(outputDirectory) + 2) * sizeof(char));
	sprintf(fullOutputPath, "%s%s%s", outputDirectory, patternFileName, underScoreMatchesTxt);
	fullOutputPath[strlen(fullOutputPath)] = '\0';

	// loop through image directory
	if(dp != NULL) 
	{
		while((ep = readdir(dp)) != NULL)
		{
			if(ep->d_type == DT_REG)
			{
				// if the current file has a .img extension
				if(isDotImgFile(ep->d_name))
				{	
					char* fullPathOfImage;
					// combine image file name with image directory path
					int lenToAllocateImagePath = 0;

					if(imgDirectory[strlen(imgDirectory) - 1] != '/')
					{
						lenToAllocateImagePath = strlen(imgDirectory) + strlen(ep->d_name) + 2; // additional + 1 for / character
						fullPathOfImage = (char*)calloc(lenToAllocateImagePath, sizeof(char));
						sprintf(fullPathOfImage, "%s/%s", imgDirectory, ep->d_name); // add slash
					}
					else
					{
						lenToAllocateImagePath = strlen(imgDirectory) + strlen(ep->d_name) + 1;
						fullPathOfImage = (char*)calloc(lenToAllocateImagePath, sizeof(char));
						sprintf(fullPathOfImage, "%s%s", imgDirectory, ep->d_name);
					}

					// find pattern matches with current image file
					getAllPatternMatchesFromImgFile(fullPathOfImage, pattern, patternMatches);

					// if pattern matches found, write them to file
					if(patternMatches->numberOfTimesFound > 0)
					{
						writePatternMatchesToOutputFile(patternDirectory, ep->d_name, patternMatches, fullOutputPath);
					}

					//free memory allocated for path to image			
					if(fullPathOfImage != NULL)
					{
						free(fullPathOfImage);
					}
				}
			}
		}
		// close image directory
		closedir(dp);
	}

	// free dynamically allocated memory
	if(patternMatches != NULL)
	{
		free(patternMatches);
	}

	if(fullOutputPath != NULL)
	{
		free(fullOutputPath);
	}
}

// finds all pattern matches for a given pattern and image file
void getAllPatternMatchesFromImgFile(const char* filePath, Matrix* pattern, PatternMatchInfo* output)
{
	// write image file to matrix for easier comparison
	Matrix* image = fileDataTo2DArray(filePath);
	int rows = image->rows;
	int i;

	int size = 0;
	int numberOfTimesFound = 0;
	
	// to sum up this approach I am doing a row by row search of the image
	// I am searching each image row for occureneces of the first row in the pattern
	// If a match is found, then I simply check if the two 3x3 rows directly under the match are the same as the last two rows of the pattern
	for(i = 0; i < rows - 2; i++)
	{
		// find occurenece of first row of pattern in the current row of the image
		char* firstPatternRowFound = strstr(image->values[i], pattern->values[0]);

		while(firstPatternRowFound != NULL)
		{
			// get the index of the first row match
			int indexFound = indexOf(image->values[i], firstPatternRowFound);

			// if second row is a match
			if(image->values[i + 1][indexFound] == pattern->values[1][0]
				&& image->values[i + 1][indexFound + 1] == pattern->values[1][1]
				&& image->values[i + 1][indexFound + 2] == pattern->values[1][2]
			)
			{
				// check if third row is a match
				if(image->values[i + 2][indexFound] == pattern->values[2][0]
					&& image->values[i + 2][indexFound + 1] == pattern->values[2][1]
					&& image->values[i + 2][indexFound + 2] == pattern->values[2][2]
				)
				{
					numberOfTimesFound++; // increase number of time pattern was found
					output->placesFound[size] = i; // add row number to output
					size++;
					output->placesFound[size] = indexFound; // add row number to output
					size++;
				}
			}
		// keep checking for multiple occurenecs of the first row within the current image row
		firstPatternRowFound = strstr(firstPatternRowFound + 1, pattern->values[0]);
		}
	}
	
	// write output metadata to PatternMatchInfo struct
	output->numberOfTimesFound = numberOfTimesFound;
	output->size = size;

	// free memory allocated for image matrix
	if(image != NULL)
	{
		freeMatrix(image);
	}		
}