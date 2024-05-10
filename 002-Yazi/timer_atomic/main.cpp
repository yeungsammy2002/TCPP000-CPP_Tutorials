#include <iostream>
#include <string>

#include <utilities/timer.h>

using namespace demo::utilities;

using std::string;

void foo()
{
    std::cout << "foo" << std::endl;
}

void bar(const string & name)
{
    std::cout << "bar: " << name << std::endl;
}

int main()
{
//    std::cout << "timer demo" << std::endl;

//    Timer t1;
//    t1.start(1000, foo);
//
//    Timer t2;
//    t2.start(2000, bar, "kitty");

    Timer t1(4);
    t1.start(1000, foo);

    Timer t2(2);
    t2.start(2000, bar, "kitty");

    std::getchar();

    return 0;
}