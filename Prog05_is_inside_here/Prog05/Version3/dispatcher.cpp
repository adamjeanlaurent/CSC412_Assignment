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

bool ProcessJobFileWithPipes(const char* jobPath, char* imagesPath, char* outputPath, char* execPath, PipeManager* pipes)
{
   bool endFound = false;

    std::string jobFilePath(jobPath);
    std::string pathToImages(imagesPath);
    std::string pathToOutput(outputPath);
    std::string pathToExecs(execPath);

    // read job file
    std::vector<Job> jobList = GetJobList(jobFilePath);

    for(Job job : jobList)
    {
        // if end found
        if(job.task == end)
        {
            endFound = true;
        }

        // convert job object to string
        std::string jobString = JobToString(job, outputPath, imagesPath);
        
        if(!jobString.empty() && !jobString.length() == 0)
        {
            // pipe job string to resident dispatcher
            pipes->PipeMessage(job.task, jobString); 
        }
    }
    return endFound;
}

std::vector<pid_t> LaunchResidentDispatchers(PipeManager* pipes, std::string pathToExecs)
{
    std::string execName = "placeholder";
    
    // get paths to resident dispatcher execs
    std::string cropResDispatcherPath = CombinePathWithFile(pathToExecs, "cropResDis");
    std::string flipHResDispatcherPath = CombinePathWithFile(pathToExecs, "flipHResDis");
    std::string flipVResDispatcherPath = CombinePathWithFile(pathToExecs, "flipVResDis");
    std::string grayResDispatcherPath = CombinePathWithFile(pathToExecs, "grayResDis");
    std::string rotateResDispatcherPath = CombinePathWithFile(pathToExecs, "rotateResDis");

    Task tasks[] = {crop, flipH, flipV, gray, rotate};

    // setup arguments for resident dispatchers
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

    // launch resident dispatchers
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