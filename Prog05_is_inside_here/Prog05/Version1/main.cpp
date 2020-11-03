#include <iostream>
#include <string>
#include <cstdio>
#include <vector>

#include "dispatcher.h"
#include "job.h"

int main(int argc, char** argv)
{
    // parse input arguments
    if(argc != 5)
    {
        // invalid arguments
        std::cout << "usage: " << argv[0] << " <pathToJobFile> <pathToImages> <pathToOutput> <pathToExecs>" << std::endl;
        return 0;
    }   

    ProcessJobFile(argv[1], argv[2], argv[3], argv[4]);

    return 0;
}