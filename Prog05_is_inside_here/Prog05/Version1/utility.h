#pragma once

#include <iostream>
#include <unistd.h>
#include <string>
#include <cstdio>
#include <vector>

#include "job.h"

class Utility
{
    public:
        Utility(std::string outputDir, std::string imageDir, Job jobObj, std::string execDir, std::string execNamePlaceholder);

        void RunTask();

    private:
        std::string outputPath;
        std::string imagePath;
        Job job;
        std::string execPath;

        void ExecCrop(std::string fullImagePath, std::string fullExecPath);
        void ExecFlipH(std::string fullImagePath, std::string fullExecPath);
        void ExecFlipV(std::string fullImagePath, std::string fullExecPath);
        void ExecGray(std::string fullImagePath, std::string fullExecPath);
        void ExecRotate(std::string fullImagePath, std::string fullExecPath);
        std::string CombinePathWithFile(std::string path, std::string filename);
};