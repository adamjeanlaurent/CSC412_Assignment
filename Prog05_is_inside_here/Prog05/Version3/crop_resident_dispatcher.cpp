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
        std::cout << "Expected Args: ./cropResDis <read-pipe> <pathToExecs>" << std::endl;
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

        else
        {
            // parse arguments
            char imagePath[500];
            char outputPath[500];
            int x;
            int y;
            int w;
            int h;

            sscanf(buffer.c_str(), "%s %s %d %d %d %d", imagePath, outputPath, &x, &y, &w, &h);

            // re construct job object
            Job job;
            job.task = crop;
            job.x = x;
            job.y= y;
            job.w = w;
            job.h = h;

            Utility util(std::string(outputPath), std::string(imagePath), job, execPath);

            // spawn process to run this utility
            pid_t id = fork();
            
            if(id == 0)
            {
                util.RunTask();
            }
            else
            {
                 std::cout << imagePath << " Has Been Cropped." << std::endl;
                processIds.push_back(id);
            }
        }
    }

    // wait for child processes to end
    for(pid_t pid : processIds)
    {
        int status = 0;
        waitpid(pid, &status, 0);
    }

    return 0;
}