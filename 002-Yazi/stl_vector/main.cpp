#include <iostream>

#include <stl/vector.h>

using namespace demo::stl;

int main()
{
//    std::cout << "vector demo" << std::endl;

    Vector<int> v;
    v.show();

    v.push_back(1);
    v.show();

    v.push_back(2);
    v.show();

    v.push_back(3);
    v.show();

    v.push_back(4);
    v.push_back(5);
    v.show();

    v.pop_back();
    v.show();

    v.reserve(6);
    v.show();

//    v.reserve(12);
//    v.show();

    v.resize(3);
    v.show();

    v.resize(7);
    v.show();

    std::cout << v.at(0) << "," << v[1] << std::endl;

    v.clear();
    v.show();

    std::cout << std::boolalpha;

    std::cout << v.empty() << std::endl;

    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    v.push_back(4);

    std::cout << v.front() << "," << v.back() << std::endl;

    Vector<int> v2;
    v2.push_back(5);
    v2.push_back(6);
    v2.show();

    v.swap(v2);
    v.show();

    v = v2;
    v.show();

    for (auto it = v.begin(); it != v.end(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    v.insert(v.begin(), 0);
    v.show();

    v.insert(v.find(1), 8);
    v.show();

    v.insert(v.find(2), 2, 8);
    v.show();

    v.insert(v.find(3), 3, 8);
    v.show();

    v.erase(v.begin());
    v.show();

    v.erase(v.find(8));
    v.show();

    v.erase(v.find(8), v.find(2));
    v.show();

    v.erase(v.find(8), v.find(3));
    v.show();

    return 0;
}