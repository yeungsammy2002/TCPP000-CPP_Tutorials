#pragma once

namespace demo {
namespace utilities {

template<typename T>
class Singleton
{
public:
    static T * instance()
    {
        static T instance;              // C++11 make local static variable thread safe
        return &instance;               // `instance` object created before running `main()`
    }

private:
    Singleton() = default;
    Singleton(const Singleton<T> &) = delete;
    Singleton<T> & operator=(const Singleton<T> &) = delete;
    ~Singleton() = default;
};

#define SINGLETON(classname)                                \
    friend class Singleton<classname>;                      \
    private:                                                \
        classname() = default;                              \
        classname(const classname &) = delete;              \
        classname & operator=(const classname &) = delete;  \
        ~classname() = default

}
}