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
#include "pipe.h"

/*
    Job of main dispatcher:
        - launch resident dispatchers
        - wait for message from pipe
        - send job to correct dispatcher through pipe
        - when end is found, tell resident dispatchers
        - wait for resident dispatchers to terminate before terminating itself
        - write back to bash script result for each piped item

    Job of other dispatchers:
        - wait for message from main dispatcher
        - parse message
        - only types of jobs should be the resident's job or an end
        - conduct job
        
        questions:
            - should these residents be execed or forked? 
*/






int main(int argc, char** argv)
{
    // parse input arguments
    if(argc != 6)
    {
        // invalid arguments
        std::cout << "usage: " << argv[0] << "./v1 <pathToImages> <pathToOutput> <pathToExecs> <nameOfPipe1> <nameOfPipe2>" << std::endl;
        return 0;
    }
    
    // setup pipes
    PipeManager pipes(
        std::string(argv[4]), 
        std::string(argv[5]), 
        "tmp/write_fliph",  
        "tmp/write_flipv",  
        "tmp/write_gray", 
        "tmp/write_crop", 
        "tmp/write_rotate");

    std::string jobFilePath; 
    char doneMessage[10] = "quit";
    char continueMessage[10] = "continue";

    bool endFound = false;

    // launch resident dispatchers 
    std::vector residentDispatchersProcessIds = LaunchResidentDispatchers(&pipes, std::string(argv[3]));

    while(!endFound)
    {
        // read from bash pipe
        jobFilePath = pipes.r_bash.Read();

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
    
    // wait for resident processes to end
    int status = 0;
    for(pid_t pid : residentDispatchersProcessIds)
    {
        waitpid(pid, &status, 0);
    }

    return 0;
}