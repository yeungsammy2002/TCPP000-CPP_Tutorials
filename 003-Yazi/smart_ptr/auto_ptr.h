#pragma once

namespace demo {
namespace utilities {

template<typename T>
class AutoPtr
{
public:
    AutoPtr() : m_data(nullptr) {}

    AutoPtr(T * data) : m_data(data) {}

    AutoPtr(AutoPtr<T> & other) : m_data(other.release()) {}

    ~AutoPtr()
    {
        if (nullptr != m_data)
        {
            delete m_data;
            m_data = nullptr;
        }
    }

    T * get() const
    {
        return m_data;
    }

    T * release()
    {
        auto data = m_data;
        m_data = nullptr;
        return data;
    }

    void reset(T * data = nullptr)
    {
        if (m_data != data)
        {
            delete m_data;
            m_data = data;
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

    AutoPtr<T> & operator=(AutoPtr<T> & other)
    {
        if (this == &other || m_data == other.m_data)
        {
            return *this;
        }
        reset(other.release());
        return *this;
    }

private:
    T * m_data;
};

}
}