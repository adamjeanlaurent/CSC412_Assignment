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
#include "utility.h"

int main(int argc, char **argv)
{
    // expected args:
    // 0: execname
    // 1: pipe to read from
    // 2: path to execs

    if(argc != 3)
    {
        std::cout << "Expected Args: ./flipVResDis <read-pipe> <pathToExecs>" << std::endl;
        return 0;
    }

    std::string execPath(argv[2]);

    // establish pipe
    Pipe readPipe;
    readPipe.fd = 0;
    readPipe.pipe = std::string(argv[1]);

    // keep reading from pipe
    // parse string
    // call new process with exec flipV
    // be careful for end

    bool endFound = false;
    std::string buffer;

    std::vector<pid_t> processIds;

    while(!endFound)
    {
        // read from pipe
        buffer = readPipe.Read();

        // parse arguments
        if(buffer == "end")
        {
            endFound = true;
        }

        else
        {
            // parse string
            char* imagePath = NULL;
            char* outputPath = NULL;

            sscanf(buffer.c_str(), "%s %s", imagePath, outputPath);

            Job job;
            job.task = flipV;

            Utility util(std::string(outputPath), std::string(imagePath), job, execPath);

            // spawn process to run this utility
            pid_t id = fork();
            
            if(id == 0)
            {
                util.RunTask();
            }
            else
            {
                processIds.push_back(id);
            }
        }
    }

    for(pid_t pid : processIds)
    {
            int status = 0;
            waitpid(pid, &status, 0);
    }

    exit(0);
}