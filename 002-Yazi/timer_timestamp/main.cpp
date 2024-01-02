#include <iostream>
#include <string>

#include <timer/timer_manager.h>
#include <utilities/singleton.h>

using namespace demo::timer;
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
//    std::cout << "timer demo timestamp version" << std::endl;

////    m_repeat = -1
//    TimerManager mgr;
//    mgr.schedule(1000, foo);
//    mgr.schedule(2000, bar, "kitty");
//
//    while (true)
//    {
//        mgr.update();
//    }

////    m_repeat > 0
//    TimerManager mgr;
//    mgr.schedule(1000, 4, foo);
//    mgr.schedule(2000, 2, bar, "kitty");
//
//    while (true)
//    {
//        mgr.update();
//    }

    auto mgr = Singleton<TimerManager>::instance();
    mgr->schedule(1000, foo);
    mgr->schedule(2000, bar, "kitty");

    while (true)
    {
        mgr->update();
    }
}