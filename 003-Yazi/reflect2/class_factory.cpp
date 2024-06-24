#include <reflect/class_factory.h>

using namespace demo::reflect;





void Object::set_classname(const string & classname)
{
    m_classname = classname;
}

const string & Object::get_classname() const
{
    return m_classname;
}





int Object::get_field_count()
{
    return Singleton<ClassFactory>::instance()->get_classfield_count(m_classname);
}

ClassField * Object::get_field(int pos)
{
    return Singleton<ClassFactory>::instance()->get_classfield(m_classname, pos);
}

ClassField * Object::get_field(const string & field_name)
{
    return Singleton<ClassFactory>::instance()->get_classfield(m_classname, field_name);
}





void ClassFactory::register_class(const string & classname, create_object func)
{
    m_classmap[classname] = func;
}

Object * ClassFactory::create_class(const string & classname)
{
    const auto it = m_classmap.find(classname);
    if (it == m_classmap.end())
    {
        return nullptr;
    }
    return it->second();
}





void ClassFactory::register_classfield(const string & classname,
                                       const string & field_name,
                                       const string & field_type,
                                       size_t offset)
{
    m_classfields[classname].push_back(new ClassField(field_name, field_type, offset));
}

int ClassFactory::get_classfield_count(const string & classname)
{
    return (int) m_classfields[classname].size();
}

ClassField * ClassFactory::get_classfield(const string & classname, const int pos)
{
    int size = (int) m_classfields[classname].size();
    if (pos < 0 || pos >= size)
    {
        return nullptr;
    }
    return m_classfields[classname][pos];
}

ClassField * ClassFactory::get_classfield(const string & classname, const string & field_name)
{
    auto & fields = m_classfields[classname];
    for (auto it = fields.begin(); it != fields.end(); ++it)
    {
        if ((*it)->name() == field_name)
        {
            return *it;
        }
    }
    return nullptr;
}





void ClassFactory::register_classmethod(const string & classname,
                                        const string & method_name,
                                        uintptr_t method)
{
    m_classmethods[classname].push_back(new ClassMethod(method_name, method));
}

int ClassFactory::get_classmethod_count(const string & classname)
{
    return (int) m_classmethods[classname].size();
}

ClassMethod * ClassFactory::get_classmethod(const string & classname, const int pos)
{
    int size = (int) m_classmethods[classname].size();
    if (pos < 0 || pos >= size)
    {
        return nullptr;
    }
    return m_classmethods[classname][pos];
}

ClassMethod * ClassFactory::get_classmethod(const string & classname, const string & method_name)
{
    auto & methods = m_classmethods[classname];
    for (auto it = methods.begin(); it != methods.end(); ++it)
    {
        if ((*it)->name() == method_name)
        {
            return *it;
        }
    }
    return nullptr;
}