#include <iostream>

#include <reflect/class_factory.h>

#include "test/a.h"
#include "test/b.h"

using namespace demo::reflect;
using namespace demo::utilities;

int main()
{
//    std::cout << "reflect demo" << std::endl;

    auto factory = Singleton<ClassFactory>::instance();
//    A * a = (A *) factory->create_object("A");
    Object * a = factory->create_object("A");
    a->show();

    Object * b = factory->create_object("B");
    b->show();

    return 0;
}
