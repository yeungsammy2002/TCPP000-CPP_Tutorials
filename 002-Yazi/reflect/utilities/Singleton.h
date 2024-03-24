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

    Singleton(const Singleton<T> &) = delete;

    Singleton<T> & operator=(const Singleton<T> &) = delete;
};

#define SINGLETON(classname)                            \
friend class Singleton<classname>;                      \
private:                                                \
    classname() = default;                              \
    ~classname() = default;                             \
    classname(const classname &) = delete;              \
    classname & operator=(const classname &) = delete;

}
}