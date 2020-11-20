#include <pthread.h>
#include <pthread.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <vector>

bool arr[5] = {false, false, false, false, false};
pthread_mutex_t lock;

void* runThread(void* arg)
{
    while(true)
    {
        int a = *(int*)arg;
        std::cout << a << std::endl;
        arr[a] = true;
        pthread_mutex_lock(&lock);
    }
     return NULL;
}

void* SyncroThreadFunc(void* arg)
{
    while(true)
    {
        // std::cout << "hi" << std::endl;
        if(arr[0] == true
        && arr[1] == true
        && arr[2] == true
        && arr[3] == true
        && arr[4] == true
        )    
        {
            arr[0] = arr[1] = arr[2] = arr[3] = arr[4] = false;
            std::cout << "unlocked!" << std::endl;
            pthread_mutex_unlock(&lock);
           // pthread_mutex_lock(&lock);
        }
    }
}

void* func(void* a)
{
    sleep(5);
    pthread_mutex_unlock(&lock);
    return NULL;
}

int main(int argc, char** argv)
{
    // pthread_mutex_lock(&lock);
    // pthread_t s;
    // pthread_create(&s, NULL, SyncroThreadFunc, NULL);
    pthread_mutex_init(&lock, NULL);
    // std::vector<pthread_t> threads;
    // int vals[] = {0,1,2,3,4};
    // for(int i = 0; i < 5; i++)
    // {
    //     pthread_t thread;
    //     pthread_create(&thread, NULL, runThread, &vals[i]);
    //     threads.push_back(thread);
    // }

    // for(int i = 0; i < 5; i++)
    // {
    //     pthread_join(threads[i], NULL);
    // }
    
    // sleep(10);
    pthread_t t;
    pthread_create(&t, NULL, func, NULL);
    pthread_mutex_lock(&lock);
    pthread_mutex_lock(&lock);
    
    std::cout << "hello" << std::endl;
    return 0;
}
