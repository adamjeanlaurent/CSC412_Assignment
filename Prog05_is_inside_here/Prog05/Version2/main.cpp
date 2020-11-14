#include <iostream>
#include <string>
#include <cstdio>
#include <vector>
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 

#include "dispatcher.h"
#include "job.h"

int main(int argc, char** argv)
{
    // parse input arguments
    if(argc != 6)
    {
        // invalid arguments
        std::cout << "usage: " << argv[0] << "./v1 <pathToImages> <pathToOutput> <pathToExecs> <nameOfPipe1> <nameOfPipe2>" << std::endl;
        return 0;
    }
    
    char doneMessage[10] = "quit\n";
    char continueMessage[10] = "continue\n";
    int readFD;
    int writeFD;
    
    char jobFilePath[500];
    bool endFound = false;

    // get pipe name
    char* readPipe = argv[4];
    char* writePipe = argv[5];

    // create named pipes
    mkfifo(readPipe, 0666);
    mkfifo(writePipe, 0666);

    while(!endFound)
    {
        // open pipe
        readFD = open(readPipe, O_RDONLY);

        // read job file path
        read(readFD, jobFilePath, 500);

        // remove newline from file path
        jobFilePath[strcspn(jobFilePath, "\n")] = 0;
        
        // process file
        endFound = ProcessJobFile(jobFilePath, argv[1], argv[2], argv[3]);
        
        // close pipe
        close(readFD);
        
        char* result;

        if(endFound)
        {
           result = doneMessage;
        }
        else
        {
            result = continueMessage;
        }
        
        // write result to pipe
        writeFD = open(writePipe, O_WRONLY);
        write(writeFD, result, strlen(result) + 1);
        // close pipe
        close(writeFD);
    }

    return 0;
}