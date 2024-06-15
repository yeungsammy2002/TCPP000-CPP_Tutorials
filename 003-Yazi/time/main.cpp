#include <iostream>

#include <utilities/time.h>

using namespace demo::utilities;

int main()
{
//    Time t;
//    t.show();

//    std::cout << "sec=" << t.seconds() << ", milliseconds=" << t.milliseconds() << std::endl;

//    std::cout << t.format("%Y-%m-%d %H:%M:%S") << std::endl;

    Time t1;
    t1.show();
    
    std::cout << t1.format("%Y-%m-%d %H:%M:%S") << std::endl;

    Time::sleep(2000);

    Time t2;

    double usec = t2 - t1;
    std::cout << usec / 1000.0 << "ms" << std::endl;

    return 0;
}