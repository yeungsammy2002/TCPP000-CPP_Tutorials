#include <iostream>

#include <utilities/timer.h>

using namespace demo::utilities;
using std::string;

void foo()
{
    std::cout << "foo" << std::endl;
}

void bar(const string & name)
{
    std::cout << "bar:" << name << std::endl;
}

int main()
{
//    std::cout << "timer demo atomic version" << std::endl;

//    Timer t1;
    Timer t1(6);
    t1.start(1000, foo);

//    Timer t2;
    Timer t2(3);
    t2.start(2000, bar, "kitty");

    std::getchar();
}