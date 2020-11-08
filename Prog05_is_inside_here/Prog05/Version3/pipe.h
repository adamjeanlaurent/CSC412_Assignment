#pragma once

#include <string>
#include <iostream>

class Pipe
{
    public:
        Pipe(int pipeFD, std::string pipePath);
        Pipe();
        void Write(std::string message);
        std::string Read();

    private:
        int fd;
        std::string pipe;
};

class PipeManager
{
    public:
        PipeManager(
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
        );

        // communcation with bash script
        Pipe r_bash;
        Pipe w_bash;

        // commnuication with residents
        Pipe r_flipH;
        Pipe w_flipH;

        Pipe r_flipV;
        Pipe w_flipV;

        Pipe r_gray;
        Pipe w_gray;

        Pipe r_crop;
        Pipe w_crop;

        Pipe r_rotate;
        Pipe w_rotate;
};
