#pragma once

#include <iostream>

#include <reflect/class_register.h>

using namespace demo::reflect;

class B : public Object
{
public:
    B() : label('B') {}

    ~B() {}

    void show()
    {
        std::cout << label << std::endl;
    }

private:
    char label;
};

REGISTER_CLASS(B);