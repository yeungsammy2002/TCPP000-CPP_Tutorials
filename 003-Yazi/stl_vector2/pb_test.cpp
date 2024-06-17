#include <iostream>

#include <stl/vector.h>
using namespace demo::stl;

#include <sys/time.h>

#include <vector>
#include <string>

using std::string;

int main()
{
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    int start_sec = tv.tv_sec;
    int start_usec = tv.tv_usec;

    const int max = 1000000;
    Vector<bool> v;
//    std::vector<bool> v;
    for (int i = 0; i < max; ++i)
    {
        v.push_back(true);
    }

    gettimeofday(&tv, nullptr);
    int end_sec = tv.tv_sec;
    int end_usec = tv.tv_usec;

    double time_diff = (end_sec - start_sec) * 1000000 + (end_usec - start_usec);
    std::cout << time_diff / 1000 / max << "ms" << std::endl;

    return 0;
}
