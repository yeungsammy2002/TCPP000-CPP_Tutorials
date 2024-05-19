#pragma once

#include <iostream>
#include <string>
#include <memory>

#include <utilities/shared_ptr.h>
#include <utilities/weak_ptr.h>

#include "test/b.h"

using namespace demo::utilities;

using std::string;

class A
{
public:
    A() = default;

    ~A()
    {
        std::cout << "A is deleted" << std::endl;
    }

//    std::shared_ptr<B> m_b;
//    std::weak_ptr<B> m_b;
//    SharedPtr<B> m_b;
    WeakPtr<B> m_b;
};