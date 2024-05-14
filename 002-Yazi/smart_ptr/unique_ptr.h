#pragma once

namespace demo {
namespace utilities {

template<typename T>
class UniquePtr
{
public:
    UniquePtr() : m_data(nullptr) {}

    UniquePtr(T * data) : m_data(data) {}

    UniquePtr(const UniquePtr<T> &) = delete;

    UniquePtr(UniquePtr<T> && other) : m_data(other.release()) {}

    ~UniquePtr()
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

    void swap(UniquePtr<T> & other)
    {
        auto data = other.m_data;
        other.m_data = m_data;
        m_data = data;
    }

    T * operator->() const
    {
        return m_data;
    }

    T & operator*() const
    {
        return *m_data;
    }

    UniquePtr<T> & operator=(const UniquePtr<T> &) = delete;

    UniquePtr<T> & operator=(UniquePtr<T> && other)
    {
        if (this == &other || m_data == other.m_data)
        {
            return *this;
        }
        reset(other.release());
        return *this;
    }

    T & operator[](const int index)
    {
        return m_data[index];
    }

    explicit operator bool() const noexcept
    {
        return nullptr != m_data;
    }

private:
    T * m_data;
};

}
}