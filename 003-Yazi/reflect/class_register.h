#pragma once

#include <reflect/class_factory.h>

namespace demo {
namespace reflect {

class ClassRegister
{
public:
    ClassRegister() = delete;

    ClassRegister(const string & classname, create_object_t method)
    {
        auto factory = Singleton<ClassFactory>::instance();
        factory->register_class(classname, method);
    }
};

#define REGISTER_CLASS(classname)                       \
    Object * create_object_##classname() {              \
        Object * obj = new classname();                 \
        return obj;                                     \
    }                                                   \
    ClassRegister class_register_##classname(#classname, create_object_##classname)

}
}