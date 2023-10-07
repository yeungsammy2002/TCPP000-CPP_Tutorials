#pragma once

#include <iostream>

#include <reflect/ClassRegister.h>

using namespace demo::reflect;

class B : public Object {
public:
    B() {}

    ~B() {}

    void show() {
        std::cout << "B" << std::endl;
    }
};

REGISTER_CLASS(B);