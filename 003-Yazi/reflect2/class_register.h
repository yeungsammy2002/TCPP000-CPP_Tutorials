#pragma once

#include <functional>

#include <reflect/class_factory.h>

namespace demo {
namespace reflect {

class ClassRegister
{
public:
    ClassRegister(const string & classname, create_object func)
    {
        Singleton<ClassFactory>::instance()->register_class(classname, func);
    }

    ClassRegister(const string & classname,
                  const string & field_name,
                  const string & field_type,
                  size_t offset)
    {
        Singleton<ClassFactory>::instance()->
                register_classfield(classname, field_name, field_type, offset);
    }

    ClassRegister(const string & classname, const string & method_name, uintptr_t method)
    {
        Singleton<ClassFactory>::instance()->register_classmethod(classname, method_name, method);
    }
};

#define REGISTER_CLASS(className)           \
    Object * createObject##className()      \
    {                                       \
        Object * obj = new className();     \
        obj->set_classname(#className);     \
        return obj;                         \
    }                                       \
    ClassRegister classRegister##className(#className, createObject##className)

#define REGISTER_CLASSFIELD(className, fieldName, fieldType)                                    \
    className className##fieldName;                                                             \
    ClassRegister classRegister##className##fieldName(  #className,                             \
                                                        #fieldName,                             \
                                                        #fieldType,                             \
                                                        (size_t)(&(className##fieldName.fieldName)) - (size_t)(&className##fieldName))

#define REGISTER_CLASSMETHOD(className, methodName, returnType, ...)                            \
    std::function<returnType(className *, ##__VA_ARGS__)> className##methodName##method         \
                                                                    = &className::methodName;   \
    ClassRegister classRegister##className##methodName( #className,                             \
                                                        #methodName,                            \
                                                        (uintptr_t)&(className##methodName##method))


}
}