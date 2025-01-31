#include <iostream>
#include <string>
#include <cstdio>
#include <vector>
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h>
#include <sys/wait.h>

#include "dispatcher.h"
#include "job.h"
#include "pipe.h"

int main(int argc, char** argv)
{
    // parse input arguments
    if(argc != 6)
    {
        // invalid arguments
        std::cout << "usage: " << argv[0] << "./v1 <pathToImages> <pathToOutput> <pathToExecs> <nameOfPipe1> <nameOfPipe2>" << std::endl;
        return 0;
    }

    std::string bashReadPipe(argv[4]);
    std::string bashWritePipe(argv[5]);

    // setup pipes
    PipeManager pipes(
        bashReadPipe,
        bashWritePipe,
        "tmp/write_fliph",  
        "tmp/write_flipv",  
        "tmp/write_gray", 
        "tmp/write_crop", 
        "tmp/write_rotate");

    std::string jobFilePath = ""; 
    char doneMessage[10] = "quit";
    char continueMessage[10] = "continue";

    bool endFound = false;

    // launch resident dispatchers 
    std::vector<pid_t> residentDispatchersProcessIds = LaunchResidentDispatchers(&pipes, std::string(argv[3]));

    while(!endFound)
    {
        // read from bash pipe
        jobFilePath = pipes.r_bash.Read();

        // process job file
        endFound = ProcessJobFileWithPipes(jobFilePath.c_str(), argv[1], argv[2], argv[3], &pipes);

        char* result;

        if(endFound)
        {
           result = doneMessage;
        }
        else
        {
            result = continueMessage;
        }

        // write result to bash
        pipes.w_bash.Write(std::string(result));
    }
    
    // wait for resident dispatcher processes to end
    int status = 0;
    for(pid_t pid : residentDispatchersProcessIds)
    {
        waitpid(pid, &status, 0);
    }
    
    return 0;
}