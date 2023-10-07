#include <reflect/ClassFactory.h>

using namespace demo::reflect;

void ClassFactory::register_class(const std::string & className, create_object method) {
    m_classMap[className] = method;
}

Object * ClassFactory::create_class(const std::string & className) {
    auto it = m_classMap.find(className);
    if (it == m_classMap.end()) return nullptr;
    return it->second();
}