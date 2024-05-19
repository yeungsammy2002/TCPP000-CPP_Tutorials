#include <reflect/class_factory.h>

using namespace demo::reflect;

void ClassFactory::register_class(const string & classname, create_object_t method)
{
    m_classmap[classname] = method;
}

Object * ClassFactory::create_object(const string & classname)
{
    const auto it = m_classmap.find(classname);
    if (it == m_classmap.end())
    {
        return nullptr;
    }
    return it->second();
}