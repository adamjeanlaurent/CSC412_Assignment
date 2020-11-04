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

    char doneMessage[5] = "done";

    int fd;

    // get pipe name
    char* myfifo = argv[4];

    // create named pipe
    mkfifo(myfifo, 0666);

    char jobFilenameBuffer[500];

    bool endFound = false;
    int lengthOfFilepath = 0;
    
    while(!endFound)
    {
        fd = open(myfifo, O_RDONLY);

        // read length of filename
        read(fd, &lengthOfFilepath, 4);

        // read file name
        read(fd, jobFilenameBuffer, lengthOfFilepath);

        endFound = ProcessJobFile(jobFilenameBuffer, argv[1], argv[2], argv[3]);
        
        // close pipe
        close(fd);
    }

    // report to bash script
    fd = open(myfifo, O_WRONLY);
    write(fd, doneMessage, strlen(doneMessage) + 1);
    close(fd);

    return 0;
}