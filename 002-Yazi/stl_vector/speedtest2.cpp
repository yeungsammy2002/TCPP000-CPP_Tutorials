#include <iostream>
#include <sys/time.h>

#include <stl/vector.h>

using namespace demo::stl;

int main()
{
//    std::cout << "speed test 2" << std::endl;

    const int max = 10000;
    Vector<int> v;
    for (int i = 0; i < max; ++i)
    {
        v.push_back(i);
    }

    struct timeval tv;
    gettimeofday(&tv, NULL);
    int start_sec = tv.tv_sec;
    int start_usec = tv.tv_usec;

    for (int i = 0; i < max; ++i)
    {
        v.erase(v.begin());
    }

    gettimeofday(&tv, NULL);
    int end_sec = tv.tv_sec;
    int end_usec = tv.tv_usec;

    double time_diff = (end_sec - start_sec) * 1000000 + (end_usec - start_usec);
    std::cout << time_diff / 1000 / max << "ms" << std::endl;

    return 0;
}