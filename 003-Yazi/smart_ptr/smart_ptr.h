#pragma once

namespace demo {
namespace utilities {

template<typename T>
class SmartPtr
{
public:
    SmartPtr() : m_data(nullptr) {}

    SmartPtr(T * data) : m_data(data) {}

    ~SmartPtr()
    {
        if (nullptr != m_data)
        {
            delete m_data;
            m_data = nullptr;
        }
    }

    T * operator->() const
    {
        return m_data;
    }

    T & operator*() const
    {
        return *m_data;
    }

private:
    T * m_data;
};

}
}