#pragma once

#include <iostream>

#include <reflect/class_register.h>

using namespace demo::reflect;

class A : public Object
{
public:
    A() {}

    ~A() {}

    void show()
    {
        std::cout << "A" << std::endl;
    }
};

REGISTER_CLASS(A);