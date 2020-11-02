#include <iostream>
#include <unistd.h>
#include <string>
#include <cstdio>
#include <vector>

#include <sys/types.h>
#include <sys/wait.h>

#include "job.h"
#include "utility.h"
#include "dispatcher.h"

void ProcessJobFile(char* jobPath, char* imagesPath, char* outputPath, char* execPath)
{
    std::string jobFilePath(jobPath);
    std::string pathToImages(imagesPath);
    std::string pathToOutput(outputPath);
    std::string pathToExecs(execPath);

    // read job file
    std::vector<Job> jobList = GetJobList(jobFilePath);

    std::vector<pid_t> processIds;

    PrintJobList(jobList);
    std::cout << "len: " << jobList.size() << std::endl;

    for(Job job : jobList)
    {
        // break if end found
        if(job.task == end)
        {
            break;
        }

        pid_t id = fork();

        if(id == 0)
        {
            // do utility stuff
            Utility util(pathToOutput, pathToImages, job, execPath);
            util.RunTask();
        }
        else
        {
            processIds.push_back(id);
        }
    }

    int status = 0;

    for(pid_t pid : processIds)
    {
        waitpid(pid, &status, 0);
    }
}