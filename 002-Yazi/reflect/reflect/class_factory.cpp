#include <reflect/class_factory.h>

using namespace demo::reflect;

void ClassFactory::register_class(const string & className, create_object_t method)
{
    m_classMap[className] = method;
}

Object * ClassFactory::create_object(const string & className)
{
    const auto it = m_classMap.find(className);
    if (it == m_classMap.end())
    {
        return nullptr;
    }
    return it->second();
}