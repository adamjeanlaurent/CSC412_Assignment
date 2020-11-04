#pragma once

#include <iostream>
#include <unistd.h>
#include <string>
#include <cstdio>
#include <vector>

#include "job.h"

/** Used by a "Utility" Process, calls image library executables. */
class Utility
{
    public:
        /**
        * Summary: Constructor for Utility class.
        * @param outputDir: Path to output folder used to pass to image library so it knows where the edited image will go.
        * @param imageDir: Path to image to perform operations on.
        * @param jobObj: The job that this utility will perform.
        * @param execDir: Path to executable to perform job.
        * @return Created Utility object.
        */ 
        Utility(std::string outputDir, std::string imageDir, Job jobObj, std::string execDir);

        /**
        * Summary: Executes job by calling relevant executable from image library.
        * @param void: none. 
        * @return void.
        */ 
        void RunTask();

    private:
        /** Path to output folder used to pass to image library so it knows where the edited image will go. */
        std::string outputPath;
        /** Path to image to perform operations on. */
        std::string imagePath;
        /** The job that this utility will perform. */
        Job job;
        /** Path to executable to perform job. */
        std::string execPath;
        /** Holds name of executable to perform job, used to pass as peramater argv[0]. */
        std::string execNamePlaceholder;

        /**
        * Summary: Executes crop executable from image library. 
        * @param fullImagePath: full path to .tga image for executable to perform operation on.
        * @param fullExecPath: full path to crop executable file. 
        * @return void.
        */ 
        void ExecCrop(std::string fullImagePath, std::string fullExecPath);

        /**
        * Summary: Executes flipH executable from image library. 
        * @param fullImagePath: full path to .tga image for executable to perform operation on.
        * @param fullExecPath: full path to flipH executable file. 
        * @return void.
        */ 
        void ExecFlipH(std::string fullImagePath, std::string fullExecPath);

        /**
        * Summary: Executes flipV executable from image library. 
        * @param fullImagePath: full path to .tga image for executable to perform operation on.
        * @param fullExecPath: full path to flipV executable file. 
        * @return void.
        */ 
        void ExecFlipV(std::string fullImagePath, std::string fullExecPath);

        /**
        * Summary: Executes gray executable from image library. 
        * @param fullImagePath: full path to .tga image for executable to perform operation on.
        * @param fullExecPath: full path to gray executable file. 
        * @return void.
        */ 
        void ExecGray(std::string fullImagePath, std::string fullExecPath);

        /**
        * Summary: Executes rotate executable from image library. 
        * @param fullImagePath: full path to .tga image for executable to perform operation on.
        * @param fullExecPath: full path to rotate executable file. 
        * @return void.
        */ 
        void ExecRotate(std::string fullImagePath, std::string fullExecPath);

        /**
        * Summary: Combines a path with a filename. ex. path = usr/bin/ filename= hello.txt => result: usr/bin/hello.txt
        * @param path: Full path to combine.
        * @param filename: Filename to combine. 
        * @return Combined string with filename and path. 
        */ 
        std::string CombinePathWithFile(std::string path, std::string filename);
};