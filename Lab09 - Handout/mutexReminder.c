//
//  mutexReminder.c
//  Not really a functional program, just an example of how to declare, initialize,
//	acquire, and release a mutex lock in the pthread library.
//
//  Created by Jean-Yves Hervé on 2020-04-13

#include <stdio.h>			// for printf
#include <pthread.h>		// for pthread_* calls
#include <string.h>			// for strerror()
#include <unistd.h>			// for usleep()
#include <stdlib.h>			// for exit()
#include <sys/types.h>		// for gettid() on Linux



//	Because mutex locks need to be accessed by the threads, you will need to either
//	declare them global in your program or store a copy of the mutex in the Thread info
//	struct

pthread_mutex_t myLock;

pthread_mutex_t myOtherLock = PTHREAD_MUTEX_INITIALIZER;

typedef struct ThreadInfo 
{
	pthread_t  threadID;
	int index;
	pthread_mutex_t myLock;
	//
	//	...
	
} ThreadInfo;



int main()
{
	//	initialize the mutex lock.  Here, I use the default initialization
	//	(the mutex is unlocked, so the first thread to ask for access will
	//	gain it.)
	pthread_mutex_init(&myLock, NULL);


	//	acquire the lock --> block if the lock is not free
	pthread_mutex_lock(&myLock);
	
	//	release the lock
	pthread_mutex_unlock(&myLock);
	
	//	basically "un-init" the lock
	pthread_mutex_destroy(&myLock);

	return 0;
}


