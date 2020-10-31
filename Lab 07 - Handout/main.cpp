//
//  main.cpp
//  Lab 07 Code Handout
//
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <list>
#include <unistd.h>
#include <fcntl.h>  
#include <cmath>
#include <cstring>

using namespace std;

using MyStruct = struct
{
	int m;
	float x;
};

int main(int argc, const char * argv[])
{
	//--------------------------------------------------------------------------
	//	Task 1: Insert here code to set up pipes and create child processes
	//--------------------------------------------------------------------------
	
	ifstream inFile;
	int numElts;

	//-------------------------------------------
	//	Reading data into a vector of MyStruct
	//-------------------------------------------
	inFile.open("data.txt");
	cout << "Reading from the file... ";
	inFile >> numElts;
	cout << numElts << " data points:" << endl;

	vector<MyStruct> myVect;
	for (int k=0; k<numElts; k++)
	{
		float x;
		int m;
		inFile >> x >> m;
		MyStruct s = {m, x};
		myVect.push_back(s);
	}
	inFile.close();
	
	cout << "Content of vector (" << myVect.size() << " elements)" << endl;
	//	Print out the contents of the vector
	for (int k=0; k<myVect.size(); k++)
	{
		cout << "\t" << myVect[k].m << " \t" << myVect[k].x << endl;
	}
	cout << endl;
	
	// create pipes

	int fd1[2];
	int fd2[2];	
	
	int errorCode1 = pipe(fd1);
	int errorCode2 = pipe(fd2);

	pid_t child_pids[2];

	if(errorCode1 == 0 && errorCode2 == 0)
	{
		// create 2 children

		pid_t id;

		for(int i = 0; i < 2; i++)
		{
			id = fork();
			
			if(id == 0)
			{
				int *currentPipe;
				// read from pipe

				if(i == 0)
					currentPipe = fd1;
				else
					currentPipe = fd2;

				// compute sum of x to power m
				double sum = 0;
				
				// read size
				unsigned int size;
				read(currentPipe[0], &size, sizeof(unsigned int));
				
				// read all values
				for(unsigned int i = 0; i < size; i++)
				{
					MyStruct ms;
					read(currentPipe[0], &ms, sizeof(MyStruct));
					sum += pow(ms.x, ms.m);
				}

				// write sum to pipe
				write(currentPipe[1], &sum, sizeof(double));
				exit(0);
			}
			
			else
			{
				child_pids[i] = id;
			}
		}

		if(id != 0)
		{
			int len = myVect.size();
			
			unsigned int halfLen;
			halfLen = len / 2;
			bool isOdd = false;

			if(len % 2 != 0)
			{
				isOdd = true;
			}

			// write first half

			// write length
			write(fd1[1], &halfLen, sizeof(unsigned int));

			// write values
			for(unsigned int i = 0; i < halfLen; i++)
			{
				MyStruct ms = myVect[i];
				write(fd1[1], &ms, sizeof(MyStruct));
			}

			// write second half

			// if odd write + 1
			if(isOdd)
				halfLen = halfLen + 1;

			int start = halfLen;

			// write length
			write(fd2[1], &halfLen, sizeof(unsigned int));

			// write values
			for(unsigned int i = start; i < len; i++)
			{
				MyStruct ms = myVect[i];
				write(fd2[1], &ms, sizeof(MyStruct));
			}

			// wait for children to finish
			for(unsigned int i = 0; i < 2; i++)
			{
				int status;
				waitpid(child_pids[i], &status, 0);
			}
			
			// read results
			double result1 = 0; 
			read(fd1[0], &result1, sizeof(double));
			double result2 = 0; 
			read(fd2[0], &result2, sizeof(double));
			
			printf("Result: %f\n", result1 + result2);
		}
	}

	return 0;
}
