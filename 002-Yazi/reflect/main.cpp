#include <iostream>

#include <reflect/ClassFactory.h>

using namespace demo::reflect;

#include "test/A.h"
#include "test/B.h"

int main() {
//    std::cout << "reflect demo" << std::endl;

    ClassFactory * factory = Singleton<ClassFactory>::instance();
    Object * a = factory->create_class("A");
    a->show();

    Object * b = factory->create_class("B");
    b->show();
}