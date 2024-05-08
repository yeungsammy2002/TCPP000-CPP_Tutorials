#pragma once

#include <iostream>

#include <reflect/class_register.h>

using namespace demo::reflect;

class A : public Object
{
public:
    A() : label('A') {}

    ~A() {}

    void show()
    {
        std::cout << label << std::endl;
    }

private:
    char label;
};

REGISTER_CLASS(A);