#pragma once

#include <reflect/class_register.h>

using namespace demo::reflect;

class A : public Object
{
public:
    A() : m_name("A"), m_age(18) {}
    ~A() {}

    void f1()
    {
        std::cout << "f1" << std::endl;
    }

    int f2(const string & name, int age)
    {
        std::cout << "f2" << ": name=" << name << ", age=" << age << std::endl;
        return 0;
    }

public:
    string m_name = "";
    int m_age;
};

REGISTER_CLASS(A);
REGISTER_CLASSFIELD(A, m_name, string);
REGISTER_CLASSFIELD(A, m_age, int);
REGISTER_CLASSMETHOD(A, f1, void);
REGISTER_CLASSMETHOD(A, f2, int, const string &, int);
