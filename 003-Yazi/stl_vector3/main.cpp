#include <iostream>

#include <string>

#include <utilities/vector.h>

using std::string;

class A
{
public:
    A() = default;
    A(const char * n) { name = n; }
    A(const string & n) { name = n; }
    A(const A & other)
    {
        name = other.name;
    }
    A(A && other)
    {
        name = other.name;
    }
    friend std::ostream & operator<<(std::ostream & os, const A & a)
    {
        os << a.name;
        return os;
    }

    A & operator=(const A & a)
    {
        name = a.name;
        return *this;
    }

    A & operator=(A && a)
    {
        name = a.name;
        return *this;
    }

    A operator+(int i)
    {
        A a;
        a.name[0] += i;
        return a;
    }

    string str() const
    {
        return name;
    }

private:
    string name = "a";
};

int main()
{
    std::cout << std::boolalpha;

//    std::cout << "vector demo" << std::endl;


//    Vector<int> v1;
//
//    v1.push_back(1);
//    v1.show();
//    v1.pop_back();
//    v1.show();


//    Vector<A> v2;
//    v2.push_back(A());
//    v2.push_back(A("b"));
//    v2.push_back(A("c"));
//    v2.push_back(A("d"));
//    v2.push_back(A("e"));
//    v2.show();
//    v2.pop_back();
//    v2.show();
//    std::cout << v2.size() << std::endl;
//    std::cout << v2.capacity() << std::endl;
//    v2.clear();
//    std::cout << v2.empty() << std::endl;
//    v2.push_back(A("f"));
//    v2.show();
//    std::cout << v2.range(0) << std::endl;
//    std::cout << v2.range(1) << std::endl;


//    Vector<int> v3;
//    v3.push_back(1);
//    v3.push_back(2);
//    v3.push_back(3);
//    v3.push_back(4);
//    v3.push_back(5);
//    std::cout << v3.at(4) << std::endl;
//    std::cout << v3[3] << std::endl;
//    std::cout << v3.front() << std::endl;
//    std::cout << v3.back() << std::endl;


//    Vector<A> v4;
//    v4.push_back(A());
//    v4.push_back(A("b"));
//    v4.push_back(A("c"));
//    v4.push_back(A("d"));
//    v4.push_back(A("e"));
//    auto v4_ptr = v4.data();
//    std::cout << *(v4_ptr + 3) << std::endl;
//    Vector<A> v5;
//    v5.push_back(A("e"));
//    v5.push_back(A("f"));
//    v5.push_back(A("g"));
//    v5.push_back(A("h"));
//    v5.push_back(A("i"));
//    v4.swap(v5);
//    v4.show();
//    v5.show();

//    Vector<int> v5;
//    v5.push_back(1);
//    v5.show();
//    v5.reserve(1);
//    v5.show();
//    v5.reserve(2);
//    v5.show();
//    v5.reserve(3);
//    v5.show();
//    v5.reserve(5);
//    v5.show();

//    Vector<int> v6;
//    v6.push_back(1);
//    v6.push_back(2);
//    v6.push_back(3);
//    v6.show();
//    v6.resize(8);
//    v6.show();
//    v6.at(3) = 4;
//    v6.show();

//    Vector<A> v7;
//    v7.push_back(A());
//    v7.push_back(A("b"));
//    v7.push_back(A("c"));
////    v7.show();
//    Vector<A> v8;
//    v8 = v7;
//    v8.show();
//    v7.show();

//    Vector<A> v8;
//    A a = "a";
//    for (int i = 0; i < 10; ++i)
//    {
//        v8.push_back(A(a + i));
//    }
//    v8.show();
//    for (auto it = v8.begin(); it != v8.end(); ++it)
//    {
//        std::cout << *it << " ";
//    }
//    std::cout << std::endl;
//    for (auto it = v8.begin(); it != v8.end(); it++)
//    {
//        std::cout << *it << " ";
//    }
//    std::cout << std::endl;
//    auto it = v8.begin() + 1;
//    std::cout << *it << std::endl;
//    auto it = v8.begin();
//    it += 1;
//    std::cout << *it << std::endl;
//    auto it = v8.end() - 1;
//    std::cout << *it << std::endl;
//    auto it = v8.end();
//    it -= 1;
//    std::cout << *it << std::endl;
//    for (auto it = v8.end() - 1; it != v8.begin() - 1; --it)
//    {
//        std::cout << it->str() << " ";
//    }
//    std::cout << std::endl;
//    for (auto it = v8.rbegin(); it != v8.rend(); ++it)
//    {
//        std::cout << it->str() << " ";
//    }
//    std::cout << std::endl;
//    auto it = v8.rbegin();
//    std::cout << *it << " " << *(it + 1) << std::endl;
//    auto it = v8.rbegin();
//    it += 2;
//    std::cout << *it << std::endl;
//    auto it = v8.rend();
//    std::cout << *(it - 1) << std::endl;
//    auto it = v8.rend();
//    it -= 1;
//    std::cout << *it << std::endl;
//    for (auto it = v8.rbegin(); it != v8.rend(); ++it)
//    {
//        std::cout << *it << " ";
//    }
//    std::cout << std::endl;
//    for (auto it = v8.rend() - 1; it != v8.rbegin(); --it)
//    {
//        std::cout << it->str() << " ";
//    }
//    std::cout << std::endl;

//    Vector<A> v9;
//    for (int i = 0; i < 26; ++i)
//    {
//        v9.push_back(A() + i);
//    }
//    v9.show();
//    v9.insert(v9.begin() + 9, A("x"));
//    v9.show();
//    v9.insert(v9.begin() + 9, 5, A("x"));
//    v9.show();
//    v9.insert(v9.begin() + 9, 10, A("none"));
//    v9.show();

//    Vector<A> v10;
//    for (int i = 0; i < 26; ++i)
//    {
//        v10.push_back(A() + i);
//    }
////    v10.show();
////    v10.erase(v10.begin());
////    v10.show();
////    v10.erase(v10.begin() + 9);
////    v10.show();
////    v10.erase(v10.end() - 1);
////    v10.show();
////    v10.erase(v10.begin() + 5, v10.begin() + 15);
////    v10.show();
//    v10.erase(v10.begin(), v10.begin() + 5);
//    v10.show();

    return 0;
}
