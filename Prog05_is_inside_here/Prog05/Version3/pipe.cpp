#include "pipe.h"
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h>

Pipe::Pipe(int pipeFD, std::string pipePath)
{
    fd = pipeFD;
    pipe = pipePath;

    mkfifo(pipe.c_str(), 0666);
}

void Pipe::Write(std::string message)
{
    const char* message_cstring = message.c_str();
    char buffer[500];

    sprintf(buffer, "%s\n", message_cstring);

    fd = open(pipe.c_str(), O_WRONLY);
    write(fd, buffer, strlen(message_cstring) + 1);
    close(fd);
}

std::string Pipe::Read()
{
    char buffer[500];

    fd = open(pipe.c_str(), O_RDONLY);
    read(fd, buffer, 500);
    
    buffer[strcspn(buffer, "\n")] = 0;
    close(fd);

    return std::string(buffer);
}

PipeManager::PipeManager(
        std::string rp_bash,
        std::string wp_bash,
        std::string rp_flipH,
        std::string wp_flipH,
        std::string rp_flipV,
        std::string wp_flipV,
        std::string rp_gray,
        std::string wp_gray,
        std::string rp_crop,
        std::string wp_crop,
        std::string rp_rotate,
        std::string wp_rotate
)
{
    r_bash = Pipe(0, rp_bash);
    w_bash = Pipe(0, wp_bash);

    r_flipH = Pipe(0, rp_flipH);
    w_flipH = Pipe(0, wp_flipH);

    r_flipV = Pipe(0, rp_flipV);
    w_flipV = Pipe(0, wp_flipV);

    r_gray = Pipe(0, rp_gray);
    w_gray = Pipe(0, wp_gray);

    r_crop = Pipe(0, rp_crop);
    w_crop = Pipe(0, wp_crop);

    r_rotate = Pipe(0, rp_rotate);
    w_rotate = Pipe(0, wp_rotate);
}

void PipeManager::PipeMessage(Task task, std::string message)
{
    switch(task)
    {
        case flipH:
            w_flipH.Write(message);
            break;
        case flipV:
            w_flipV.Write(message);
            break;
         case gray:
            w_gray.Write(message);
            break;
         case crop:
            w_crop.Write(message);
            break;
         case rotate:
            w_rotate.Write(message);
            break;
         case end:
            // write end to all pipes
            w_flipH.Write(message);
            w_flipV.Write(message);
            w_gray.Write(message);
            w_crop.Write(message);
            w_rotate.Write(message);
            break;
    }
    exit(0);
}