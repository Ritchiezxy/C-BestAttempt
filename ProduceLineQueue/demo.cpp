#include <iostream>
#include <ctime>
#include <cstdlib>
#include "ProduceLineQueue.hpp"

using namespace std;

ProdLineQueue<int> intPQueue;
const int Count = 100;
mutex ioMutex;

void consumer()
{
	for (int i = 0; i < Count; ++i)
	{
		int value = intPQueue.pop();
		ioMutex.lock();
		cout << __func__ << " consumed:" << value << endl;
		ioMutex.unlock();
	}
}

void producer()
{
	srand(time(NULL));
	for (int i = 0; i < Count; ++i)
	{
		int value = rand() % 1000;
		intPQueue.push(value);
		ioMutex.lock();
		cout << __func__ << " produced:" << value << endl;
		ioMutex.unlock();
	}
}

void producer1()
{
	srand(time(NULL));
	for (int i = 0; i < Count; ++i)
	{
		int value = rand() % 1000;
		intPQueue.push(value);
		ioMutex.lock();
		cout << __func__ << " produced:" << value << endl;
		ioMutex.unlock();
	}
}

void consumer1()
{
	for (int i = 0; i < Count; ++i)
	{
		int value = intPQueue.pop();
		ioMutex.lock();
		cout << __func__ << " consumed:" << value << endl;
		ioMutex.unlock();
	}
}

int main(void) 
{
	//intPQueue.setMaxSize(10);
	std::thread threadC1(consumer);
	std::thread threadP1(producer);
	std::thread threadC2(consumer1);
	std::thread threadP2(producer1);
	threadC1.join();
	threadP1.join();
	threadC2.join();
	threadP2.join();
	return 0;
}
