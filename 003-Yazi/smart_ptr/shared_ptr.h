#pragma once

namespace demo {
namespace utilities {

template<typename T>
class SharedPtr
{
    template<typename Y> friend
    class WeakPtr;

public:
    SharedPtr() : m_data(nullptr), m_count(nullptr) {}

    SharedPtr(T * data) : m_data(data), m_count(nullptr)
    {
        if (nullptr != data)
        {
            m_count = new int(1);
        }
    }

    SharedPtr(const SharedPtr<T> & other) : m_data(other.m_data), m_count(other.m_count)
    {
        if (nullptr != other.m_data)
        {
            ++(*m_count);
        }
    }

    SharedPtr(SharedPtr<T> && other) : m_data(other.m_data), m_count(other.m_count)
    {
        other.m_data = nullptr;
        other.m_count = nullptr;
    }

    ~SharedPtr()
    {
        if (nullptr != m_data)
        {
            --(*m_count);
            if (0 >= *m_count)
            {
                delete m_data;
                delete m_count;
            }
            m_data = nullptr;
            m_count = nullptr;
        }
    }

    T * get() const
    {
        return m_data;
    }

    void reset(T * data = nullptr)
    {
        if (m_data != data)
        {
            --(*m_count);
            if (0 >= *m_count)
            {
                delete m_data;
                delete m_count;
            }
            m_data = nullptr;
            m_count = nullptr;
        }
        m_data = data;
        if (nullptr != data)
        {
            m_data = new int(1);
        }
    }

    void swap(SharedPtr<T> & other)
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

    bool unique() const
    {
        if (nullptr == m_data)
        {
            return false;
        }
        return 1 == *m_count;
    }

    T * operator->() const
    {
        return m_data;
    }

    T & operator*() const
    {
        return *m_data;
    }

    SharedPtr<T> & operator=(const SharedPtr<T> & other)
    {
        if (&other == this || other.m_data == m_data)
        {
            return *this;
        }
        if (nullptr != m_data)
        {
            --(*m_count);
            if (0 >= *m_count)
            {
                delete m_data;
                delete m_count;
            }
        }
        m_data = other.m_data;
        m_count = other.m_count;
        if (nullptr != other.m_data)
        {
            ++(*m_count);
        }
        return *this;
    }

    SharedPtr<T> & operator=(SharedPtr<T> && other)
    {
        if (&other == this || m_data == other.m_data)
        {
            return *this;
        }
        if (nullptr != m_data)
        {
            --(*m_count);
            if (0 >= *m_count)
            {
                delete m_data;
                delete m_count;
            }
        }
        m_data = other.m_data;
        m_count = other.m_count;
        other.m_data = nullptr;
        other.m_count = nullptr;
        return *this;
    }

    explicit operator bool() const noexcept
    {
        return nullptr != m_data;
    }

private:
    T * m_data;
    int * m_count;
};

}
}