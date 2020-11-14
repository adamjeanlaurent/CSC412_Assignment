#include <pthread.h>
#include <pthread.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <iostream>

void* runThread(void* arg)
{
    std::cout << "helo world" << std::endl;

    return NULL;
}

int main(int argc, char** argv)
{
    pthread_t thread;
    pthread_create(&thread, NULL, runThread, NULL);
    std::cout << "goodbye world" << std::endl;

    return 0;
}