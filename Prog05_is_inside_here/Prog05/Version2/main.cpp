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
    if(argc != 5)
    {
        // invalid arguments
        std::cout << "usage: " << argv[0] << "./v1 <pathToImages> <pathToOutput> <pathToExecs> <nameOfPipe>" << std::endl;
        return 0;
    }

    char doneMessage[10] = "quit\n";
    char continueMessage[10] = "continue\n";
    int fd;
    char jobFilePath[500];
    bool endFound = false;

    // get pipe name
    char* myfifo = argv[4];

    // create named pipe
    mkfifo(myfifo, 0666);

    while(!endFound)
    {
        // open pipe
        fd = open(myfifo, O_RDONLY);

        // read job file path
        read(fd, jobFilePath, 500);

        // remove newline from file path
        jobFilePath[strcspn(jobFilePath, "\n")] = 0;

        // process file
        endFound = ProcessJobFile(jobFilePath, argv[1], argv[2], argv[3]);
        
        // close pipe
        close(fd);

        if(endFound)
        {
            break;
        }

        else
        {
            // write continue result
            fd = open(myfifo, O_WRONLY);
            write(fd, continueMessage, strlen(continueMessage) + 1);

            // close pipe
            close(fd);   
        }
    }

    // write done result
    fd = open(myfifo, O_WRONLY);
    write(fd, doneMessage, strlen(doneMessage) + 1);
    close(fd);

    return 0;
}