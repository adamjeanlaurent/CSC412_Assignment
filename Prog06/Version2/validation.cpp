#include "validation.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>

bool validateArguments(const char** argv, int argc)
{
	// verify correct number of arguments
	if(argc != 4)
	{
		return showArgumentRules();
	}

	// convert args to int
	int height = atoi(argv[1]);
	int width = atoi(argv[2]);
	int numThreads = atoi(argv[3]);

	// verify width and height are larger than 5
	if(width <= 5 || height <= 5)
	{
		return showArgumentRules();
	}

	// verify number of threads is non-zero and non-negative
	if(numThreads < 1)
	{
		return showArgumentRules();
	}

	// verify number of threads is not larger than height
	if(numThreads > height)
	{
		return showArgumentRules();
	}

	return true;
}

bool showArgumentRules()
{
	std::cout << "Incorrect Arguments!" << std::endl;
	std::cout << "Rules: " << std::endl;
	std::cout << "usage: ./cells <width> <height> <numThreads>" << std::endl;
	std::cout << "Where: " << "width > 5 & height > 5 & height >= numThreads & numThreads > 0" << std::endl;

	return false;
}