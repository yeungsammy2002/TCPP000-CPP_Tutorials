#pragma once

#include <iostream>
#include <memory>

#include <utilities/shared_ptr.h>

using namespace demo::utilities;

class A;

class B
{
public:
    B() = default;

    ~B()
    {
        std::cout << "B is deleted" << std::endl;
    }

//    std::shared_ptr<A> m_a;
    SharedPtr<A> m_a;
};