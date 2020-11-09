#pragma once

#include <string>
#include <iostream>
#include "job.h"

class Pipe
{
    public:
        Pipe(int pipeFD, std::string pipePath);
        Pipe();
        void Write(std::string message);
        std::string Read();
        
        int fd;
        std::string pipe;

};

class PipeManager
{
    public:
        PipeManager(
            std::string rp_bash,
            std::string wp_bash,
            std::string wp_flipH,
            std::string wp_flipV,
            std::string wp_gray,
            std::string wp_crop,
            std::string wp_rotate
        );

        void PipeMessage(Task task, std::string message);

        // communcation with bash script
        Pipe r_bash;
        Pipe w_bash;

        // commnuication with residents
        Pipe w_flipH;

        Pipe w_flipV;

        Pipe w_gray;

        Pipe w_crop;

        Pipe w_rotate;
};
