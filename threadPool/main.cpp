#include <iostream>
#include "ThreadPool.h"

std::mutex mu;
int a = 0;
void sharedPrint(const char *msg)
{
	std::lock_guard<std::mutex> lg(mu);
	std::cout << msg;
	a++;
}

int main()
{
	ThreadPool tp;

	for(int i =0; i<100; i++)
	{
		tp.enque
		([]() 
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
				sharedPrint("thread 1\n");
			}
		);
	}
	

	tp.join();
	std::this_thread::sleep_for(std::chrono::seconds(1));
	std::cout << a;

	std::cin.get();
	return 0;
}