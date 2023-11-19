#include <iostream>

#include <stl/Vector.h>

using namespace demo::stl;

int main()
{
//    std::cout << "stl demo" << std::endl;

//    std::cout << "\ncheck if Vector(), push_back(), size(), capacity() and show() are working:\n";
//    Vector<int> v;
//    v.push_back(1);
//    v.push_back(2);
//    v.push_back(3);
//    v.show();
//
//    std::cout << "\ncheck if m_capacity extendable:\n";
//    v.push_back(4);
//    v.push_back(5);
//    v.show();
//
//    std::cout << "\ncheck if pop_back() is working:\n";
//    v.pop_back();
//    v.show();

//    std::cout << "\ncheck if empty() is working and pop_back()'s vector empty error is working:\n";
//    Vector<int> v1;
//    v1.pop_back();

//    std::cout << "\ncheck if reserve() remain unchanged if given size is less than m_capacity:\n";
//    Vector<int> v2;
//    v2.push_back(1);
//    v2.push_back(2);
//    v2.push_back(3);
//    v2.show();
//    v2.reserve(3);
//    v2.show();
//
//    std::cout << "\ncheck if reserve() change m_capacity if size is larger than existing m_capacity:\n";
//    v2.reserve(6);
//    v2.show();

//    std::cout << "\ncheck if resize() is working when given size is less than m_capacity\n";
//    Vector<int> v3;
//    v3.push_back(1);
//    v3.push_back(2);
//    v3.push_back(3);
//    v3.show();
//    v3.resize(2);
//    v3.show();
//
//    std::cout << "\ncheck if resize() extending elements if given size is more than m_capacity\n";
//    v3.resize(6);
//    v3.show();

//    std::cout << "\ncheck if at() and subscript operator overloading are working:\n";
//    Vector<int> v4;
//    v4.push_back(1);
//    v4.push_back(2);
//    v4.push_back(3);
//    std::cout << "index 0:" << v4.at(0) << ", index 2:" << v4[2] << std::endl;
//
//    std::cout << "\ncheck if at()'s out of range error is working if given index is invalid:\n";
////    std::cout << v4.at(-1) << std::endl;
//    std::cout << v4[3] << std::endl;

//    std::cout << "\ncheck if clear() is working:\n";
//    Vector<int> v5;
//    v5.push_back(1);
//    v5.show();
//    v5.clear();
//    v5.show();

//    std::cout << "\ncheck if front() and back() are working:\n";
//    Vector<int> v6;
//    v6.push_back(1);
//    v6.push_back(2);
//    v6.push_back(3);
//    std::cout << "first element:" << v6.front() << ", last element:" << v6.back() << std::endl;

//    std::cout << "\ncheck if front() and back()'s vector empty error are working if m_size is 0:\n";
//    Vector<int> v7;
////    std::cout << v7.front() << std::endl;
//    std::cout << v7.back() << std::endl;

//    std::cout << "\ncheck if swap() is working:\n";
//    Vector<int> v8;
//    v8.push_back(1);
//    v8.push_back(2);
//    v8.push_back(3);
//    Vector<int> v9;
//    v8.swap(v9);
//    v8.show();
//    v9.show();

//    std::cout << "\ncheck if data() is working:\n";
//    Vector<int> v10;
//    v10.push_back(88);
//    int * it = v10.data();
//    std::cout << *it << std::endl;

//    Vector<int> x;
//    for (int i = 0; i < 10; ++i)
//        x.push_back(i + 1);
//    x.show();
////    x.erase(x.begin());
////    x.erase(x.end());
//    x.erase(x.find(4));
//    x.show();

    Vector<int> x;
    for (int i = 0; i < 10; ++i)
        x.push_back(i + 1);
    x.erase(x.find(3), x.find(8));
    x.show();
}