#include <vector>
#include <iostream>
#include <ctime>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <cstring>

std::string ReadFromPipe()
{
    std::string pipe = "./tmp/bash_to_c";
    char buffer[500];
    int fd = open(pipe.c_str(), O_RDONLY);
    read(fd, buffer, 500);
    buffer[strcspn(buffer, "\n")] = 0;
    buffer[strlen(buffer)] = '\0';
    close(fd);
    return std::string(buffer);
}

int main(void)
{
    while(true)
    {
       std::string res = ReadFromPipe();
       std::cout << "message: " << res << std::endl;
    }
   
}