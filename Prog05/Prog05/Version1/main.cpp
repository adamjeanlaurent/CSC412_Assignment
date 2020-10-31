#include <iostream>
#include <unistd.h>
#include <string>
#include <cstdio>
#include <vector>

#include "job.h"


int main(int argc, char** argv)
{
    // parse input arguments
    if(argc != 4)
    {
        // invalid arguments
        std::cout << "usage: " << argv[0] << " <pathToJobFile> <pathToImages> <pathToOutput>" << std::endl;
        return 0;
    }   
    
    std::string jobFilePath(argv[1]);
    std::string pathToImages(argv[2]);
    std::string pathToOutput(argv[3]);

   // read job file
    std::vector<Job> jobList = GetJobList(jobFilePath);
    
    return 0;
}