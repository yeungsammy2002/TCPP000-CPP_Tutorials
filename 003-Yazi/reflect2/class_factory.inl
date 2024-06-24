
template<typename T>
void Object::get(const string & field_name, T & value)
{
    ClassField * field = Singleton<ClassFactory>::instance()->
            get_classfield(m_classname, field_name);

    if (nullptr == field)
    {
        return;
    }

    size_t offset = field->offset();
    value = *((T *) ((unsigned char *) (this) + offset));
}

template<typename T>
void Object::set(const string & field_name, const T & value)
{
    ClassField * field = Singleton<ClassFactory>::instance()->
            get_classfield(m_classname, field_name);

    if (nullptr == field)
    {
        return;
    }
    
    size_t offset = field->offset();
    *((T *) ((unsigned char *) (this) + offset)) = value;
}

template<typename R, typename ...Args>
R Object::call(const string & method_name, Args && ... args)
{
    ClassFactory * factory = Singleton<ClassFactory>::instance();
    ClassMethod * method = factory->get_classmethod(m_classname, method_name);
    if (nullptr == method)
    {
        return R();
    }
    auto func = method->method();
    typedef std::function<R(decltype(this), Args...)> classmethod;
    return (*((classmethod *) func))(this, args...);
}

