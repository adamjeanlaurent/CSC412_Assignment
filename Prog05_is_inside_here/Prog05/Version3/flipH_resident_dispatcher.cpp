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
    if(argc != 3)
    {
        std::cout << "Expected Args: ./flipHResDis <read-pipe> <pathToExecs>" << std::endl;
        return 0;
    }

    ResidentDispatcherProcessTask(argv[1], argv[2], flipH);

    return 0;
}