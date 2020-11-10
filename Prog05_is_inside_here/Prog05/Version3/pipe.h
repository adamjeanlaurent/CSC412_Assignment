#pragma once

#include <string>
#include <iostream>
#include "job.h"

/** Resusable Pipe class with methods for reading and writing. */
class Pipe
{
    public:
        /** 
         * summary: Constructor that calls mkfifo(). 
         * @param pipeFD: file descriptor.
         * @param pipePath: pipe to pipe.
         * @return Pipe object called with mkfifo().
         */
        Pipe(int pipeFD, std::string pipePath);
        
        /** 
         * summary: Default constructor. 
         * @return Pipe object.
         */
        Pipe();
        
         /** 
         * summary: Writes message to pipe, using linux's write() method under the hood.
         * @param message: string to send through pipe.
         * @return void.
         */
        void Write(std::string message);

         /** 
         * summary: Reads message from pipe, using linux's read() method under the hood. 
         * @return string read from pipe.
         */
        std::string Read();
        
        /** File Descriptor. */
        int fd;
        /** Path To Pipe. */
        std::string pipe;

};

/** Class that holds Pipe objects for communication with bash script, resident dispatchers and main dispatcher. */
class PipeManager
{
    public:
        /**
         * Summary: Constructor for PipeManager r(read) and w(write) is from the perscpetive of the main dispatcher.
         * @param rp_bash: Path to read bash pipe.
         * @param wp_bash: Path to write bash pipe.
         * @param wp_flipH: Path to write flipH resident dispatcher pipe.
         * @param wp_flipV: Path to write flipV resident dispatcher pipe.
         * @param wp_gray: Path to write gray resident dispatcher pipe.
         * @param wp_crop: Path to write crop resident dispatcher pipe.
         * @param wp_rotate: Path to write rotate resident dispatcher pipe.
         * @return pipe manager object.
         */
        PipeManager(
            std::string rp_bash,
            std::string wp_bash,
            std::string wp_flipH,
            std::string wp_flipV,
            std::string wp_gray,
            std::string wp_crop,
            std::string wp_rotate
        );

        /**
         * Summary: Helper method to send messages through pipes depending on the task
         * @param task: task to perform.
         * @param message: message to send through pipe.
         * @return void.
         */ 
        void PipeMessage(Task task, std::string message);

        /** Bash read pipe. */
        Pipe r_bash;
        /** Bash write pipe. */
        Pipe w_bash;
        /** flipH resident dispatcher write pipe. */
        Pipe w_flipH;
        /** flipV resident dispatcher write pipe. */
        Pipe w_flipV;
        /** gray resident dispatcher write pipe. */
        Pipe w_gray;
        /** crop resident dispatcher write pipe. */
        Pipe w_crop;
        /** rotate resident dispatcher write pipe. */
        Pipe w_rotate;
};