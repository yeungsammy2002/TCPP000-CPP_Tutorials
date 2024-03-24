#pragma once

#include <map>
#include <string>

#include <utilities/singleton.h>

using namespace demo::utilities;

using std::string;

namespace demo {
namespace reflect {

class Object
{
public:
    Object() {}

    virtual ~Object() {}

    virtual void show() = 0;
};

typedef Object * (* create_object_t)(void);

class ClassFactory
{
    SINGLETON(ClassFactory);
public:
    void register_class(const string & className, create_object_t method);

    Object * create_object(const string & className);

private:
    std::map<string, create_object_t> m_classMap;
};

}
}