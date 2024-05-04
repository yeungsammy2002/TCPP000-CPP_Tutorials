#pragma once

namespace demo {
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

    ~Singleton() = default;

    Singleton(const Singleton<T> & other) = delete;

    Singleton<T> & operator=(const Singleton<T> & other) = delete;
};

#define SINGLETON(classname)                                    \
    friend class Singleton<classname>;                          \
private:                                                        \
    classname();                                                \
    ~classname();                                               \
    classname & operator=(const classname & other) = delete;    \
    classname(const classname & other) = delete

}
}