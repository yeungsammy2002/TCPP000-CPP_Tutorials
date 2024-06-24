// a simple demonstration of reflect mechanism

#include <string>

using std::string;

class A
{
};

class B
{
};

void * create_class(const string & class_name)
{
    if ("A" == class_name)
    {
        return new A();
    } else if ("B" == class_name)
    {
        return new B();
    } else
    {
        return nullptr;
    }
}

