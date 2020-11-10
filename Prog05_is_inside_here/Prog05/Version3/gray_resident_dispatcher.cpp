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
#include "utility.h"

int main(int argc, char **argv)
{
    // expected args:
    // 0: execname
    // 1: pipe to read from
    // 2: path to execs

    if(argc != 3)
    {
        std::cout << "Expected Args: ./grayResDis <read-pipe> <pathToExecs>" << std::endl;
        return 0;
    }

    std::string execPath(argv[2]);

    // establish pipe
    Pipe readPipe;
    readPipe.fd = 0;
    readPipe.pipe = std::string(argv[1]);

    bool endFound = false;
    std::string buffer;

    std::vector<pid_t> processIds;

    while(!endFound)
    {
        // read from pipe
        buffer = readPipe.Read();

        if(buffer.length() == 0)
            continue;

        if(buffer == "end")
        {
            endFound = true;
        }

        // parse arguments
        else
        {
            char imagePath[500];
            char outputPath[500];

            sscanf(buffer.c_str(), "%s %s", imagePath, outputPath);

            // reconstruct job object
            Job job;
            job.task = gray;

            Utility util(std::string(outputPath), std::string(imagePath), job, execPath);

            // spawn process to run this utility
            pid_t id = fork();
            
            if(id == 0)
            {
                util.RunTask();
            }
            else
            {
                std::cout << imagePath << " Has Been Grayed." << std::endl;
                processIds.push_back(id);
            }
        }
    }

    // wait for child processes
    for(pid_t pid : processIds)
    {
            int status = 0;
            waitpid(pid, &status, 0);
    }

    return 0;
}