#include <iostream>
#include <string>

#include <timer/timer_manager.h>

using namespace demo::timer;

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
//    std::cout << "timer 2 demo" << std::endl;

    auto mgr = Singleton<TimerManager>::instance();
//    mgr->schedule(1000, foo);
//    mgr->schedule(2000, bar, "kitty");

    mgr->schedule(1000, 4, foo);
    mgr->schedule(2000, 2, bar, "kitty");

    while (true)
    {
        mgr->update();
    }
}