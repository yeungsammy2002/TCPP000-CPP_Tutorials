#pragma once

#include <utilities/shared_ptr.h>

namespace demo {
namespace utilities {

template<typename T>
class WeakPtr
{
public:
    WeakPtr() : m_data(nullptr), m_count(nullptr) {}

    WeakPtr(const SharedPtr<T> & sp) : m_data(sp.m_data), m_count(sp.m_count) {}

    WeakPtr(const WeakPtr<T> & other) : m_data(other.m_data), m_count(other.m_count) {}

    WeakPtr(WeakPtr<T> && other) : m_data(other.m_data), m_count(other.m_count)
    {
        other.reset();
    }

    ~WeakPtr()
    {
        reset();
    }

    void reset()
    {
        m_data = nullptr;
        m_count = nullptr;
    }

    bool expired() const
    {
        return (nullptr == m_data) || (0 >= *m_count);
    }

    SharedPtr<T> & lock() const
    {
        if (expired())
        {
            return SharedPtr<T>();
        }
        SharedPtr<T> sp;
        sp.m_data = m_data;
        sp.m_count = m_count;
        if (nullptr != m_data)
        {
            ++(*m_count);
        }
        return sp;
    }

    void swap(WeakPtr<T> & other)
    {
        auto data = other.m_data;
        auto count = other.m_count;

        other.m_data = m_data;
        other.m_count = m_count;

        m_data = data;
        m_count = count;
    }

    int use_count() const
    {
        if (nullptr == m_data)
        {
            return 0;
        }
        return *m_count;
    }

    WeakPtr<T> & operator=(const SharedPtr<T> & sp)
    {
        m_data = sp.m_data;
        m_count = sp.m_count;
        return *this;
    }

    WeakPtr<T> & operator=(const WeakPtr<T> & other)
    {
        if (&other == this || other.m_data == m_data)
        {
            return *this;
        }
        m_data = other.m_data;
        m_count = other.m_count;
        return *this;
    }

    WeakPtr<T> & operator=(WeakPtr<T> && other)
    {
        if (this == &other || m_data == other.m_data)
        {
            return *this;
        }
        m_data = other.m_data;
        m_count = other.m_count;
        other.reset();
        return *this;
    }

private:
    T * m_data;
    int * m_count;
};

}
}