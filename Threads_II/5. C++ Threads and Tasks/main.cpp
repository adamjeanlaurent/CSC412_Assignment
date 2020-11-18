//
//  main.cpp
//  Threads and Tasks
//
//  Created by Jean-Yves Hervé [Local] on 2016-04-27.
//	Revised 2020-03-29, 2020-11-11

//-----------------------------------------------
//	VERSION --> 1 to 5
//-----------------------------------------------
#define VERSION_1	1
#define VERSION_2	2
#define VERSION_3	3
#define VERSION_4	4
#define VERSION_5	5
//	Pick one
#define VERSION		VERSION_1



#if VERSION == VERSION_1

	//------------------------------------------
	//	new style pthreads (no parameter) C++11
	//------------------------------------------

	#include <iostream>
	#include <thread>
	#include <unistd.h>

	void threadFunc(void)
	{
		std::cout << "In the thread function" << std::endl;
		
		for (int k=0; k<100; k++)
		{
			std::cout << "\tthread counting " << k << std::endl;
			usleep(20000);
			if (k==30)
				return;
		}

//	do stuff
		usleep(2000000);

		std::cout << "Thread terminated" << std::endl;
	}

	int main()
	{
		std::thread thr(threadFunc);

		std::cout << "Thread launched from main\n";
		
		//	do stuff
		for (int k=0; k<10; k++)
		{
			std::cout << "main thread counting " << k << std::endl;
			usleep(500000);
		}
		//	Wait for the thread to terminate
		thr.join();
		std::cout << "Thread joined in main\n";

		return 0;
	}

#elif VERSION == VERSION_2

	//-----------------------------------------------
	//	new style pthreads (with parameter) C++11
	//-----------------------------------------------

	#include <iostream>
	#include <thread>
	#include <unistd.h>

	void threadFunc(int val)
	{
		std::cout << "In the thread function, received val=" << val << std::endl;
		
		//	do stuff
		usleep(2000000);

		std::cout << "Thread terminated" << std::endl;
	}

	int main()
	{
		std::thread thr(threadFunc, 8);
		//	argh!
		//		std::thread thr(threadFunc);

		std::cout << "Thread launched from main\n";
		
		//	do stuff

		//	Wait for the thread to terminate
		thr.join();
		std::cout << "Thread joined in main\n";

		return 0;
	}

#elif VERSION == VERSION_3

	//-----------------------------------------------
	//	new style pthreads (with i/o param)  C++11
	//-----------------------------------------------

	#include <iostream>
	#include <thread>
	#include <vector>
	#include <unistd.h>
	#include <pthread.h>

	using ThreadStruct = struct {
					int param1;
					float param2;
					float param3;
					int outputVal1;
					std::vector<float> outputVal2;
					//	only needed when I use pthread to compare
					pthread_t threadID;
	} ;

	void threadFunc(void* data) noexcept;

	void* threadFuncWrap(void* data) {
		threadFunc(data);
		return nullptr;
	}

	void threadFunc(void* dataV) noexcept
	{
		ThreadStruct* data = static_cast<ThreadStruct*>(dataV);
		
		std::cout << "In the thread function" << std::endl;
		
		std::cout	<< "Received data struct with param1 = " << data->param1
					<< ", param2 = " << data->param2
					<< ", param3 = " << data->param3 << std::endl;
		
		//	do stuff
		usleep(2000000);
		
		data->outputVal1 = 2*data->param1;
		data->outputVal2 = std::vector<float>(data->outputVal1);
		for (int k=0; k<data->outputVal1; k++)
			data->outputVal2[k] = 1000.f*data->param2 + k*data->param3;
		
		//	noting to return
		
		std::cout << "The thread terminated" << std::endl;
	}

	void anotherThreadFunc(ThreadStruct& data)
	{
		std::cout << "In the thread function" << std::endl;
		
		std::cout	<< "Received data struct with param1 = " << data.param1
					<< ", param2 = " << data.param2
					<< ", param3 = " << data.param3 << std::endl;
		
		//	do stuff
		
		data.outputVal1 = 2*data.param1;
		data.outputVal2 = std::vector<float>(data.outputVal1);
		for (int k=0; k<data.outputVal1; k++)
			data.outputVal2[k] = 1000.f*data.param1 + k*data.param2;

		//	noting to return
		
		std::cout << "The thread terminated" << std::endl;
	}

	int main()
	{
		ThreadStruct myData = {	4, 2.5f, 12.5f	//	params 1, 2, 3
								};
	//	c++ thread
	#if 0
//		std::thread thr(threadFunc, &myData);
		std::thread thr2(anotherThreadFunc, myData);

		std::cout << "Thread launched from main\n";
		
		//	do stuff

		//	Wait for the thread to terminate
		thr.join();
	//	pthread
	#else
		int errCode = pthread_create(&(myData.threadID), NULL, threadFuncWrap, &myData);

		std::cout << "Thread launched from main\n";

		void* retVal;
		errCode = pthread_join(myData.threadID, &retVal);
	#endif
	
		std::cout << "Joined in main thread, got data back:" << std::endl;
		std::cout << "\treturn1:\t" << myData.outputVal1 << std::endl;
		std::cout << "\treturn2:\t";
		for (int k=0; k<myData.outputVal2.size(); k++)
		{
			if (k>0)
				std::cout << "\t\t\t";
			std::cout << myData.outputVal2[k] << std::endl;
		}

		return 0;
	}

	//	Problem with direct creation of software threads:  limited resource
	//	If too many software threads are created --> std::system_error thrown
	//	[even if the thread's function is noexcept]

#elif VERSION == VERSION_4

	//-----------------------------------------------------
	//	new style task C++14 [adapted from Cplusplus.com]
	//-----------------------------------------------------

	// launch::async vs launch::deferred
	#include <iostream>     // std::cout
	#include <future>       // std::async, std::deferred std::future, std::launch
	#include <chrono>       // std::chrono::milliseconds
	#include <thread>       // std::this_thread::sleep_for
	#include <unistd.h>

	void print_ten (char c, int ms)
	{
	  for (int i=0; i<10; ++i)
	  {
		std::this_thread::sleep_for(std::chrono::milliseconds(ms));
		std::cout << c;
	  }
		std::cout << "Task terminated\n";
	}

	int main ()
	{
		//	I separated declaration from initialization so that the first
		//	launch does not appear (unfairly) more complex than the others.
		std::future<void> foo;
		std::future<void> bar;

		std::cout << "with explicit launch::async:\n";
		foo = std::async(std::launch::async, print_ten, '*', 100);
		bar = std::async(std::launch::async, print_ten, '@', 200);

		// async "get" (wait for foo and bar to be ready):
		bar.get();
		foo.get();
		std::cout << "\n\n";

		std::cout << "with explicit launch::deferred:\n";
		foo = std::async(std::launch::deferred, print_ten, '*', 100);
		bar = std::async(std::launch::deferred, print_ten, '@', 200);
		// deferred "get" (perform the actual calls):
		foo.get();
		bar.get();
		std::cout << "\n\n";

		std::cout << "with default launch:\n";
		foo = std::async(print_ten, '*', 100);
		bar = std::async(print_ten, '@', 200);
		// deferred "get" (perform the actual calls):
		foo.get();
		bar.get();
		std::cout << "\n\n";

//
//		//	This is where auto comes handy to make code readable
//		std::cout << "the same, with auto declaration:\n";
//		auto foo2 = std::async(print_ten, '*', 100);
//		auto bar2 = std::async(print_ten, '@', 200);
//		// deferred "get" (perform the actual calls):
//		foo2.get();
//		bar2.get();
//		std::cout << "\n\n";

		return 0;
	}

#elif VERSION == VERSION_5

	//----------------------------------------------
	//	new style task, with return value C++14
	//----------------------------------------------

	// launch::async vs launch::deferred
	#include <iostream>     // std::cout
	#include <future>       // std::async, std::future, std::launch
	#include <chrono>       // std::chrono::milliseconds
	#include <thread>
	#include <vector>

	//	silly function
	template <typename T>
	std::vector<T> makePalindrome(const std::vector<T>& vec)
	{
		std::vector<T> outVec;
		for (int k=0; k<vec.size(); k++)
		{
			outVec.push_back(vec[k]);
			std::this_thread::sleep_for(std::chrono::milliseconds(300));
		}
		for (int k=vec.size()-1; k>=0; k--)
		{
			outVec.push_back(vec[k]);
			std::this_thread::sleep_for(std::chrono::milliseconds(300));
		}
		
		return outVec;
	}

	int main ()
	{
		std::vector<int>v1 {12, 34, 35, 22};

		std::cout << "Creating an explicitly asynchronous task" << std::endl;

		auto task1 = std::async(std::launch::async, makePalindrome<int>, v1);
		std::cout << "Waiting in the main" << std::flush << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		std::cout << "End wait" << std::flush << std::endl;

		auto out1 = task1.get();
		std::cout << "Values returned by task 1: ";
		for(auto val : out1)
		{
			std::cout << val << "  ";
		}
		std::cout << "\n\n";
		
		std::cout << "Creating an explicitly deferred task" << std::endl;

		auto task2 = std::async(std::launch::deferred, makePalindrome<int>, v1);
		std::cout << "Waiting in the main" << std::flush << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		std::cout << "End wait" << std::flush << std::endl;
		

		auto out2 = task2.get();
		std::cout << "Values returned by task 2: ";
		for(auto val : out2)
		{
			std::cout << val << "  ";
		}
		std::cout << "\n\n";
		
		
		
		return 0;
	}

#else
	#error invalid version number
#endif
