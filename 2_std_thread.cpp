#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <functional>
#include <sstream>


using std::string;

//void show(const string & msg)
//{
//	std::cout << msg << std::endl;
//}
//
//class A
//{
//public:
//	using Fn = void(*)(const string &);
//	operator Fn()
//	{
//		return show;
//	}
//};
//
//int main()
//{
//	std::thread t1(A(), "hello world");		// std::thread accept any callable objects
//
//	t1.join();								// std::thread::join() to recycle resources
//
//	return 0;
//}


// Demo of using std::this_thread::sleep_for(), std::this_thread::get_id()
//void show(const string & msg)
//{
//	std::this_thread::sleep_for(std::chrono::seconds(5));
//	std::cout << "child thread ID: " << std::this_thread::get_id() << std::endl;
//}
//
//int main()
//{
//	std::thread t1(show, "hello world");
//	std::this_thread::sleep_for(std::chrono::seconds(2));
//	// std::this_thread::sleep_for() is cross-platform function and standardize parameter in order to replace Windows's Sleep()
//	// and Linux's sleep() function
//	std::cout << "main thread ID: " << std::this_thread::get_id() << std::endl;
//
//	t1.join();
//	return 0;
//}


// Demo of using std::this_thread::sleep_until()
//int main()
//{
//	auto after3s = std::chrono::system_clock::now() + std::chrono::seconds(3);
//	std::this_thread::sleep_until(after3s);
//	std::cout << "hello world" << std::endl;
//
//	std::string time_str = "2025-06-13 01:19:00";			// GMT+8, 01:19:00 means 09:19:00
//	std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds> tp;
//	std::istringstream ss{ time_str };
//	ss >> std::chrono::parse("%Y-%m-%d %H:%M:%S", tp);		// only available C++20
//
//	std::this_thread::sleep_until(tp);
//	std::cout << "2025-16-13 09:19:00" << std::endl;
//
//	return 0;
//}


// Demo of Producer-Consumer Model - Concurrency Pattern
// class Producer_Consumer_Model
// {
// public:
// 	void produce(int num)
// 	{
// 		std::lock_guard<std::mutex> lock(m_mutex);
// 		for (int i = 0; i < num; ++i)
// 		{
// 			static int n = 1;
// 			string data = string("message") + std::to_string(n++);
// 			m_queue.push(data);
// 		}
// 		//m_cond.notify_one();							// only one thread get notify
// 		m_cond.notify_all();							// all thread get notify
// 	}
//
// 	void consume()
// 	{
// 		while (true)
// 		{
// 			std::unique_lock<std::mutex> lock(m_mutex);
// 			while (m_queue.empty())
// 			{
// 				std::cout << "thread " << std::this_thread::get_id() << " waiting for incoming data..." << std::endl;
// 				m_cond.wait(lock);
// 			}
//
// 			string data = m_queue.front();
// 			m_queue.pop();
//
// 			std::cout << "thread " << std::this_thread::get_id() << " has incoming data: " << data << std::endl;
// 			lock.unlock();
//
// 			std::this_thread::sleep_for(std::chrono::seconds(1));	// assuming processing data
// 		}
// 	}
//
// private:
// 	std::mutex m_mutex;
// 	std::condition_variable m_cond;
// 	std::queue<string> m_queue;							// default underlying container
// 	//std::queue<string, std::deque<string>> m_queue;	// of std::queue is std::deque
// };
//
// int main()
// {
// 	Producer_Consumer_Model pcm;
// 	std::thread consumer1(&Producer_Consumer_Model::consume, &pcm);
// 	std::thread consumer2(&Producer_Consumer_Model::consume, &pcm);
// 	std::thread consumer3(&Producer_Consumer_Model::consume, &pcm);
//
// 	std::this_thread::sleep_for(std::chrono::seconds(3));
//
// 	pcm.produce(5);
//
// 	consumer1.join();
// 	consumer2.join();
// 	consumer3.join();
//
// 	return 0;
// }