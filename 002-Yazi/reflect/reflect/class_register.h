#pragma once

#include <string>

#include <reflect/class_factory.h>

using std::string;

namespace demo {
namespace reflect {

class ClassRegister
{
public:
    ClassRegister(const string & className, create_object_t method)
    {
        auto factory = Singleton<ClassFactory>::instance();
        factory->register_class(className, method);
    }
};

#define REGISTER_CLASS(classname)                                   \
    Object * create_object_##classname()                            \
    {                                                               \
        Object * obj = new classname();                             \
        return obj;                                                 \
    }                                                               \
    ClassRegister class_register_##classname(#classname, create_object_##classname)

}
}