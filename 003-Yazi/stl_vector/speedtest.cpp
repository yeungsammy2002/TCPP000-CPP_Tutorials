#include <iostream>
#include <sys/time.h>
#include <vector>

#include <stl/vector.h>

using namespace demo::stl;

int main()
{
//    std::cout << "speed test" << std::endl;

    Vector<bool> v;
//    std::vector<bool> v;
    const int max = 10000;

    struct timeval tv;
    gettimeofday(&tv, NULL);
    int start_sec = tv.tv_sec;
    int start_usec = tv.tv_usec;

    for (int i = 0; i < max; ++i)
    {
        v.insert(v.begin(), true);
    }

    gettimeofday(&tv, NULL);
    int end_sec = tv.tv_sec;
    int end_usec = tv.tv_usec;

    double time_diff = (end_sec - start_sec) * 1000000 + (end_usec - start_usec);
    std::cout << time_diff / max / 1000 << "ms" << std::endl;

    return 0;
}