#pragma once

#include <string>
#include <map>
#include <vector>
#include <functional>

#include <utilities/singleton.h>
#include <reflect/class_field.h>
#include <reflect/class_method.h>

using namespace demo::utilities;

using std::string;

namespace demo {
namespace reflect {


class Object
{
public:
    Object() = default;
    virtual ~Object() = default;

    void set_classname(const string & classname);
    const string & get_classname() const;

    int get_field_count();
    ClassField * get_field(int pos);
    ClassField * get_field(const string & field_name);

    template<typename T>
    void get(const string & field_name, T & value);

    template<typename T>
    void set(const string & field_name, const T & value);

    template<typename R = void, typename ... Args>
    R call(const string & method_name, Args && ... args);

private:
    string m_classname;
};

typedef Object * (* create_object)();

class ClassFactory
{
    SINGLETON(ClassFactory);

public:
    void register_class(const string & classname, create_object func);
    Object * create_class(const string & classname);

    void register_classfield(const string & classname,
                             const string & field_name,
                             const string & field_type,
                             size_t offset);
    int get_classfield_count(const string & classname);
    ClassField * get_classfield(const string & classname, const int pos);
    ClassField * get_classfield(const string & classname, const string & field_name);

    void register_classmethod(const string & classname,
                              const string & method_name,
                              uintptr_t method);
    int get_classmethod_count(const string & classname);
    ClassMethod * get_classmethod(const string & classname, const int pos);
    ClassMethod * get_classmethod(const string & classname, const string & method_name);

private:
    std::map<string, create_object> m_classmap;
    std::map<string, std::vector<ClassField *>> m_classfields;
    std::map<string, std::vector<ClassMethod *>> m_classmethods;
};

#include <reflect/class_factory.inl>

}
}