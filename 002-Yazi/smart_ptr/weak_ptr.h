#pragma once

#include <utilities/shared_ptr.h>

namespace demo {
namespace utilities {

template<typename T>
class WeakPtr
{
public:
    WeakPtr();

    WeakPtr(const SharedPtr<T> & sp);

    WeakPtr(const WeakPtr<T> & other);

    WeakPtr(WeakPtr<T> && other) noexcept;

    ~WeakPtr();

    void reset();

    bool expired() const;

    SharedPtr<T> lock() const;

    void swap(WeakPtr<T> & other);

    int use_count() const;

    WeakPtr<T> & operator=(const SharedPtr<T> & sp);

    WeakPtr<T> & operator=(const WeakPtr<T> & other);

    WeakPtr<T> & operator=(WeakPtr<T> && other);

private:
    T * m_data;
    int * m_count;
};

template<typename T>
WeakPtr<T>::WeakPtr() : m_data(nullptr), m_count(nullptr) {}

template<typename T>
WeakPtr<T>::WeakPtr(const SharedPtr<T> & sp) : m_data(sp.m_data), m_count(sp.m_count) {}

template<typename T>
WeakPtr<T>::WeakPtr(const WeakPtr<T> & other) : m_data(other.m_data), m_count(other.m_count) {}

template<typename T>
WeakPtr<T>::WeakPtr(WeakPtr<T> && other) noexcept
{
    m_data = other.m_data;
    m_count = other.m_count;
    other.m_data = nullptr;
    other.m_count = nullptr;
}

template<typename T>
WeakPtr<T>::~WeakPtr()
{
    m_data = nullptr;
    m_count = nullptr;
}

template<typename T>
void WeakPtr<T>::reset()
{
    m_data = nullptr;
    m_count = nullptr;
}

template<typename T>
bool WeakPtr<T>::expired() const
{
    return !m_count || (*m_count) <= 0;
}

template<typename T>
SharedPtr<T> WeakPtr<T>::lock() const
{
    if (expired())
        return SharedPtr<T>();

    SharedPtr<T> sp;
    sp.m_data = m_data;
    sp.m_count = m_count;

    if (nullptr != m_count)
        ++(*m_count);

    return sp;
}

template<typename T>
void WeakPtr<T>::swap(WeakPtr<T> & other)
{
    T * data = other.m_data;
    int * count = other.m_count;
    other.m_data = m_data;
    other.m_count = m_count;
    m_data = data;
    m_count = count;
}

template<typename T>
int WeakPtr<T>::use_count() const
{
    if (nullptr == m_data)
        return 0;

    return *m_count;
}

template<typename T>
WeakPtr<T> & WeakPtr<T>::operator=(const SharedPtr<T> & sp)
{
    m_data = sp.m_data;
    m_count = sp.m_count;
    return *this;
}

template<typename T>
WeakPtr<T> & WeakPtr<T>::operator=(const WeakPtr<T> & other)
{
    if (this == &other)
        return *this;

    m_data = other.m_data;
    m_count = other.m_count;
    return *this;
}

template<typename T>
WeakPtr<T> & WeakPtr<T>::operator=(WeakPtr<T> && other)
{
    if (this == &other)
        return *this;

    m_data = other.m_data;
    m_count = other.m_count;
    other.m_data = nullptr;
    other.m_count = nullptr;
    return *this;
}

}
}