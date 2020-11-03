#include <iostream>
#include <unistd.h>
#include <string>
#include <cstdio>
#include <vector>

#include "job.h"
#include "utility.h"

Utility::Utility(std::string outputDir, std::string imageDir, Job jobObj, std::string execDir)
{
    outputPath = outputDir;
    imagePath = imageDir;
    job = jobObj;
    execPath = execDir;
    execNamePlaceholder = "placeholder";
}

void Utility::RunTask()
{
    std::string fullImagePath = CombinePathWithFile(imagePath, job.imageFilename);
    std::string fullExecPath = CombinePathWithFile(execPath, TaskEnumToString(job.task));

    switch(job.task)
    {
        case flipH:
            ExecFlipH(fullImagePath, fullExecPath);
            break;
        case flipV:
            ExecFlipV(fullImagePath, fullExecPath);
            break;
        case gray:
            ExecGray(fullImagePath, fullExecPath);
            break;
        case crop:
            ExecCrop(fullImagePath, fullExecPath);
            break;
        case rotate:
            ExecRotate(fullImagePath, fullExecPath);
            break;
        default:
            break;
    }

    exit(0);
}

void Utility::ExecCrop(std::string fullImagePath, std::string fullExecPath)
{
    // convert numbers to strings
    const char* x_char = std::to_string(job.x).c_str();
    const char* y_char = std::to_string(job.y).c_str();
    const char* w_char = std::to_string(job.w).c_str();
    const char* h_char = std::to_string(job.h).c_str();
    
    // reason for const_cast
    // https://stackoverflow.com/questions/190184/execv-and-const-ness
    char* const arguments[] = 
    {
        const_cast<char* const>(execNamePlaceholder), 
        const_cast<char* const>(fullImagePath.c_str()), 
        const_cast<char* const>(outputPath.c_str()), 
        const_cast<char* const>(x_char), 
        const_cast<char* const>(y_char), 
        const_cast<char* const>(w_char), 
        const_cast<char* const>(h_char), 
        NULL
    };

    // build exec path
    execv(fullExecPath.c_str(), arguments);
}

void Utility::ExecFlipH(std::string fullImagePath, std::string fullExecPath)
{
        char* const arguments[] = 
        {
            const_cast<char* const>(execNamePlaceholder), 
            const_cast<char* const>(fullImagePath.c_str()),
            const_cast<char* const>(outputPath.c_str()),
            NULL
        };

        execv(fullExecPath.c_str(), arguments);
}

void Utility::ExecFlipV(std::string fullImagePath, std::string fullExecPath)
{
    char* const arguments[] = 
    {
        const_cast<char* const>(execNamePlaceholder), 
        const_cast<char* const>(fullImagePath.c_str()),
        const_cast<char* const>(outputPath.c_str()),
        NULL
    };
    execv(fullExecPath.c_str(), arguments);
}

void Utility::ExecGray(std::string fullImagePath, std::string fullExecPath)
{
    char* const arguments[] = 
    {
        const_cast<char* const>(execNamePlaceholder), 
        const_cast<char* const>(fullImagePath.c_str()),
        const_cast<char* const>(outputPath.c_str()),
        NULL
    };
    execv(fullExecPath.c_str(), arguments);
}

void Utility::ExecRotate(std::string fullImagePath, std::string fullExecPath)
{
    char* const arguments[] = 
    {
        const_cast<char* const>(execNamePlaceholder), 
        const_cast<char* const>(job.rotation.c_str()),
        const_cast<char* const>(fullImagePath.c_str()),
        const_cast<char* const>(outputPath.c_str()),
        NULL
    };
    execv(fullExecPath.c_str(), arguments);
}

std::string Utility::CombinePathWithFile(std::string path, std::string filename) 
{
    int len = path.length() - 1;
    bool hasSlash;

    if(path[len] == '/')
        hasSlash = true;
    else
        hasSlash = false;
    
    std::string combinedStr = path;

    if(hasSlash == false)
    {
        combinedStr = combinedStr + '/';
    }

    combinedStr = combinedStr + filename;

    return combinedStr;
}