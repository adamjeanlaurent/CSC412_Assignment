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
#include "pipe.h"

bool ProcessJobFile(char* jobPath, char* imagesPath, char* outputPath, char* execPath)
{
    bool endFound = false;

    std::string jobFilePath(jobPath);
    std::string pathToImages(imagesPath);
    std::string pathToOutput(outputPath);
    std::string pathToExecs(execPath);
    
    // read job file
    std::vector<Job> jobList = GetJobList(jobFilePath);

    std::vector<pid_t> processIds;

    for(Job job : jobList)
    {
        // break if end found
        if(job.task == end)
        {
            endFound = true;
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

    return endFound;
}

bool ProcessJobFileWithPipes(const char* jobPath, char* imagesPath, char* outputPath, char* execPath, PipeManager* pipes)
{
   bool endFound = false;

    std::string jobFilePath(jobPath);
    std::string pathToImages(imagesPath);
    std::string pathToOutput(outputPath);
    std::string pathToExecs(execPath);

    // read job file
    std::vector<Job> jobList = GetJobList(jobFilePath);

    std::vector<pid_t> processIds;

    for(Job job : jobList)
    {
        // if end found
        if(job.task == end)
        {
            endFound = true;
        }

        pid_t id = fork();

        if(id == 0)
        {
            // job data to string 
            std::string jobString = JobToString(job, outputPath, imagesPath);
            
            // do pipe stuff
            pipes->PipeMessage(job.task, jobString); 
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

std::vector<pid_t> LaunchResidentDispatchers(PipeManager* pipes, std::string pathToExecs)
{
    std::string execName = "placeholder";
    
    std::string cropResDispatcherPath = CombinePathWithFile(pathToExecs, "cropResDis");
    std::string flipHResDispatcherPath = CombinePathWithFile(pathToExecs, "flipHResDis");
    std::string flipVResDispatcherPath = CombinePathWithFile(pathToExecs, "flipVResDis");
    std::string grayResDispatcherPath = CombinePathWithFile(pathToExecs, "grayResDis");
    std::string rotateResDispatcherPath = CombinePathWithFile(pathToExecs, "rotateResDis");

    Task tasks[] = {crop, flipH, flipV, gray, rotate};

    char* const cropArgs[] = 
    {
        const_cast<char* const>(execName.c_str()),
        const_cast<char* const>(pipes->w_crop.pipe.c_str()),
        const_cast<char* const>(pathToExecs.c_str()),
        NULL 
    };

    char* const flipHArgs[] = 
    {
        const_cast<char* const>(execName.c_str()),
        const_cast<char* const>(pipes->w_flipH.pipe.c_str()),
        const_cast<char* const>(pathToExecs.c_str()),
        NULL 
    };

    char* const flipVArgs[] = 
    {
        const_cast<char* const>(execName.c_str()),
        const_cast<char* const>(pipes->w_flipV.pipe.c_str()),
        const_cast<char* const>(pathToExecs.c_str()),
        NULL 
    };

    char* const grayArgs[] = 
    {
        const_cast<char* const>(execName.c_str()),
        const_cast<char* const>(pipes->w_gray.pipe.c_str()),
        const_cast<char* const>(pathToExecs.c_str()),
        NULL 
    };

    char* const rotateArgs[] = 
    {
        const_cast<char* const>(execName.c_str()),
        const_cast<char* const>(pipes->w_rotate.pipe.c_str()),
        const_cast<char* const>(pathToExecs.c_str()),
        NULL 
    };
    
    std::vector<pid_t> processIds;

    for(Task task : tasks)
    {
        pid_t id = fork();

        if(id == 0)
        {
            switch(task)
            {
                case crop: 
                    execv(cropResDispatcherPath.c_str(), cropArgs);
                    break;
                case flipH:
                    execv(flipHResDispatcherPath.c_str(), flipHArgs);
                    break;
                case flipV:
                    execv(flipVResDispatcherPath.c_str(), flipVArgs);
                    break;
                case gray:
                    execv(grayResDispatcherPath.c_str(), grayArgs);
                    break;
                case rotate:
                    execv(rotateResDispatcherPath.c_str(), rotateArgs);
                    break;
            }
        }
        else
        {
            processIds.push_back(id);
        }
    }
    return processIds;
}