#pragma once

namespace regulus {
namespace utilities {

template<typename T>
class Singleton
{
public:
	static T * instance()
	{
		static T instance;
		return &instance;
	}
private:
	Singleton() = default;
	Singleton(const Singleton<T> &) = delete;
	Singleton & operator=(const Singleton<T> &) = delete;
	~Singleton() = default;
};

#define SINGLETON(classname)							\
friend Singleton<classname>;							\
private:												\
	classname();										\
	classname(const classname &) = delete;				\
	classname & operator=(const classname &) = delete;	\
	~classname();

}
}