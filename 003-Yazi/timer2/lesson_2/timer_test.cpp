#include <iostream>
#include <string>

#include <utilities/timer.h>

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
//    Timer t1;
//    t1.start(1000, foo);
//
//    Timer t2;
//    t2.start(1500, bar, "kitty");

    Timer t1(2);
    t1.start(1000, foo);

    Timer t2(3);
    t2.start(1500, bar, "kitty");

    std::getchar();

    return 0;
}