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

    if(jobList.size() == 0)
    {
        std::cout << "Empty Job List" << std::endl;
    }

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

void ResidentDispatcher(char* pipe, char* pathToExecs, Task task)
{
    std::string execPath(pathToExecs);

    // establish pipe
    Pipe readPipe;
    readPipe.fd = 0;
    readPipe.pipe = std::string(pipe);

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
            char imagePath[500];
            char outputPath[500];
            char rotation[500];

            Job job;

            // parse message from pipe into a job object
            switch(task)
            {
                case flipH:
                    sscanf(buffer.c_str(), "%s %s", imagePath, outputPath);
                    break;
                case flipV:
                    sscanf(buffer.c_str(), "%s %s", imagePath, outputPath);
                    break;
                case crop: 
                    int x;
                    int y;
                    int w;
                    int h;
                    sscanf(buffer.c_str(), "%s %s %d %d %d %d", imagePath, outputPath, &x, &y, &w, &h);
                    job.x = x;
                    job.y = y;
                    job.w = w;
                    job.h = h;
                    break;

                case gray:
                    sscanf(buffer.c_str(), "%s %s", imagePath, outputPath);
                    break;

                case rotate:
                    sscanf(buffer.c_str(), "%s %s %s", imagePath, rotation, outputPath);
                    job.rotation = std::string(rotation);
                    break;
            }

            job.task = task;

            Utility util(std::string(outputPath), std::string(imagePath), job, execPath);

            // spawn process to utility
            pid_t id = fork();
            
            if(id == 0)
            {
                util.RunTask();
            }
            else
            {
                std::cout << TaskEnumToString(task) <<  ": " << imagePath << std::endl;
                processIds.push_back(id);
            }
        }
    }

    // wait for child processes to finish
    for(pid_t pid : processIds)
    {
        int status = 0;
        waitpid(pid, &status, 0);
    }
}