#include <iostream>
#include <string>
#include <vector>
#include <sys/time.h>

using std::string;

#include <stl/Vector.h>

using namespace demo::stl;

int main()
{
    std::cout << "stl vector" << std::endl;

    std::cout << "\ncheck if Vector(), push_back(), size(), capacity() and show() are working:\n";
    Vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    v.show();

    std::cout << "\ncheck if m_capacity extendable:\n";
    v.push_back(4);
    v.push_back(5);
    v.show();

    std::cout << "\ncheck if pop_back() is working:\n";
    v.pop_back();
    v.show();

//    std::cout << "\ncheck if empty() is working and pop_back()'s vector empty error is working:\n";
//    Vector<int> v1;
//    v1.pop_back();

    std::cout << "\ncheck if reserve() remain unchanged if given size is less than m_capacity:\n";
    Vector<int> v2;
    v2.push_back(1);
    v2.push_back(2);
    v2.push_back(3);
    v2.show();
    v2.reserve(3);
    v2.show();

    std::cout << "\ncheck if reserve() change m_capacity if size is larger than existing m_capacity:\n";
    v2.reserve(6);
    v2.show();

    std::cout << "\ncheck if resize() is working when given size is less than m_capacity\n";
    Vector<int> v3;
    v3.push_back(1);
    v3.push_back(2);
    v3.push_back(3);
    v3.show();
    v3.resize(2);
    v3.show();

    std::cout << "\ncheck if resize() extending elements if given size is more than m_capacity\n";
    v3.resize(6);
    v3.show();

    std::cout << "\ncheck if at() and subscript operator overloading are working:\n";
    Vector<int> v4;
    v4.push_back(1);
    v4.push_back(2);
    v4.push_back(3);
    std::cout << "index 0:" << v4.at(0) << ", index 2:" << v4[2] << std::endl;

//    std::cout << "\ncheck if at()'s out of range error is working if given index is invalid:\n";
////    std::cout << v4.at(-1) << std::endl;
//    std::cout << v4[3] << std::endl;

    std::cout << "\ncheck if clear() is working:\n";
    Vector<int> v5;
    v5.push_back(1);
    v5.show();
    v5.clear();
    v5.show();

    std::cout << "\ncheck if front() and back() are working:\n";
    Vector<int> v6;
    v6.push_back(1);
    v6.push_back(2);
    v6.push_back(3);
    std::cout << "first element:" << v6.front() << ", last element:" << v6.back() << std::endl;

//    std::cout << "\ncheck if front() and back()'s vector empty error are working if m_size is 0:\n";
//    Vector<int> v7;
//    std::cout << v7.front() << std::endl;
////    std::cout << v7.back() << std::endl;

    std::cout << "\ncheck if swap() is working:\n";
    Vector<int> v8;
    v8.push_back(1);
    v8.push_back(2);
    v8.push_back(3);
    Vector<int> v9;
    v8.swap(v9);
    std::cout << "v8:";
    v8.show();
    std::cout << "v9:";
    v9.show();

    std::cout << "\ncheck if data() is working:\n";
    Vector<int> v10;
    v10.push_back(88);
    int * it = v10.data();
    std::cout << *it << std::endl;

    std::cout << "\ncheck if assignment operator overloading is working when m_capacity is larger than other.m_size:\n";
    Vector<int> v11;
    v11.push_back(1);
    v11.push_back(2);
    v11.push_back(3);
    v11.push_back(4);
    v11.show();
    Vector<int> v11a;
    v11a.push_back(88);
    v11 = v11a;
    v11.show();

    std::cout
            << "\ncheck if assignment operator overloading if working when m_capacity is smaller then other.m_capacity:\n";
    Vector<int> v12;
    v12.push_back(1);
    v12.push_back(2);
    v12.push_back(3);
    v12.show();
    Vector<int> v12a;
    for (int i = 1; i <= 10; ++i)
        v12a.push_back(i);
    v12 = v12a;
    v12.show();

    std::cout
            << "\ncheck if equality operator overloading, inequality operator overloading, pre-increment operator overloading,\n"
               "post-increment operator overloading, plus operator overloading, plus composite assignment operator overloading,\n"
               "minus operator overloading, minus composite assignment operator overloading, substract operator overloading,\n"
               "and dereference operator overloading are working:\n" << std::endl;
    Vector<int> v13;
    for (int i = 1; i <= 10; ++i)
        v13.push_back(i);
//    v13.show();
    std::cout << std::boolalpha;
    Vector<int>::Iterator first_a(&v13.at(0));
    Vector<int>::Iterator first_b = v13.begin();
    std::cout << "first_a == first_b: " << (first_a == first_b) << std::endl;

    Vector<int>::Iterator first_c(&v13[8]);
    std::cout << "first_a != first_c: " << (first_a != first_c) << std::endl;

    std::cout << "dereference (++first_a): " << *(++first_a) << std::endl;

    std::cout << "dereference (first_a++): " << *(first_a++) << std::endl;
    std::cout << "next line of (first_a++): " << *first_a << std::endl;

    std::cout << "dereference (first_b + 3): " << *(first_b + 3) << std::endl;

    first_b += 3;
    std::cout << "first_b += 3, then dereference first_b: " << *first_b << std::endl;

    std::cout << "dereference (first_b - 2): " << *(first_b - 2) << std::endl;

    first_b -= 1;
    std::cout << "first_b -= 1, then dereference first_b: " << *first_b << std::endl;

    std::cout << "first_b - begin(): " << (first_b - v13.begin()) << std::endl;

    std::cout << "\ncheck if arrow operator overloading is working:\n";
    class Person
    {
    public:
        Person() : m_name("") {}

        Person(string name) : m_name(name) {}

        string m_name;
    };
    Person p("Jack");
    Vector<Person> v14;
    v14.push_back(p);
    Vector<Person>::Iterator it_person = v14.begin();
    std::cout << "first Person object's name: " << it_person->m_name << std::endl;

    std::cout
            << "\ncheck if Iterator(), begin(), end(), != operator overloading, pre ++ operator overloading, dereference operator overloading are working:";
    Vector<int> v15;
    for (int i = 0; i < 10; ++i)
        v15.push_back(i + 1);
    for (Vector<int>::Iterator it = v15.begin(); it != v15.end(); ++it)
        std::cout << *it << " ";
    std::cout << std::endl;

    std::cout << "\ncheck if find(), insert() is working:\n";
    Vector<int> v16;
    for (int i = 0; i < 10; ++i)
        v16.push_back(i + 1);
    std::cout << "v16.find(8): " << *(v16.find(8)) << std::endl;
    v16.insert(v16.find(8), 88);
    std::cout << "insert 88 before 8:\n";
    v16.show();

    std::cout << "\ncheck if insert() multiple elements is working:\n";
    Vector<int> v17;
    for (int i = 0; i < 10; ++i)
        v17.push_back(i + 1);
    v17.insert(v17.find(6), 6, 66);
    std::cout << "insert 6 numbers of 66 before 6:\n";
    v17.show();
    v17.insert(v17.find(8), 8, 88);
    std::cout << "insert 8 numbers of 88 before 8:\n";
    v17.show();

    std::cout << "\ncheck if erase() is working:\n";
    Vector<int> v18;
    for (int i = 0; i < 10; ++i)
        v18.push_back(i + 1);
    v18.show();
    v18.erase(v18.find(6));
    v18.show();

    std::cout << "\ncheck if erase() multiple elements is working:\n";
    Vector<int> v19;
    for (int i = 0; i < 10; ++i)
        v19.push_back(i + 1);
    v19.show();
    v19.erase(v19.find(3), v19.find(8));
    v19.show();

    std::cout << "\ninsert() performance test Vector when compared to std::vector:\n";
    const int max = 10000;
    Vector<bool> v20;
    std::vector<bool> v21;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    int start_sec = tv.tv_sec;
    int start_usec = tv.tv_usec;
    for (int i = 0; i < max; ++i)
        v20.insert(v20.begin(), true);
    gettimeofday(&tv, NULL);
    int end_sec = tv.tv_sec;
    int end_usec = tv.tv_usec;
    double time_diff_Vector = (end_sec - start_sec) * 1000000 + (end_usec - start_usec);

    gettimeofday(&tv, NULL);
    start_sec = tv.tv_sec;
    start_usec = tv.tv_usec;
    for (int i = 0; i < max; ++i)
        v21.insert(v21.begin(), true);
    gettimeofday(&tv, NULL);
    end_sec = tv.tv_sec;
    end_usec = tv.tv_usec;
    double time_diff_stl_vector = (end_sec - start_sec) * 1000000 + (end_usec - start_usec);

    std::cout << "Vector: " << time_diff_Vector / 1000 / max << "ms" << std::endl;
    std::cout << "std::vector: " << time_diff_stl_vector / 1000 / max << "ms" << std::endl;
}