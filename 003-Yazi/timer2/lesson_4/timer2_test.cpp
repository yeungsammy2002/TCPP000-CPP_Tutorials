#include <iostream>
#include <string>

#include <utilities/timer2.h>
#include <utilities/timer_manager.h>

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
    TimerManager mgr;
    mgr.schedule(1000, foo);
    mgr.schedule(1500, bar, "kitty");

    while (true)
    {
        mgr.update();
    }

    return 0;
}