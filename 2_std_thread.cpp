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