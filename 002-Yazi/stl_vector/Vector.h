#pragma once

#include <stdexcept>
#include <iostream>
#include <cstring>

namespace demo {
namespace stl {

template<typename T>
class Vector
{
public:
    Vector();

    ~Vector();

    void push_back(const T & value);

    void pop_back();

    int size() const;

    int capacity() const;

    void show() const;

    void reserve(int size);

    void resize(int size);

    T & at(int index);

    T & operator[](int index);

    bool empty() const;

    void clear();

    T & front();

    T & back();

    void swap(Vector<T> & other);

    T * data();

    Vector<T> & operator=(const Vector<T> & other);

    class Iterator
    {
    public:
        Iterator();

        Iterator(T * pointer);

        ~Iterator() = default;

        bool operator==(const Iterator other) const;

        bool operator!=(const Iterator other) const;

        Iterator & operator=(const Iterator other) const;

        Iterator & operator++();

        Iterator operator++(int);

        Iterator operator+(int i) const;

        Iterator & operator+=(int i);

        Iterator operator-(int i) const;

        Iterator & operator-=(int i);

        int operator-(const Iterator other) const;

        T & operator*();

        T * operator->();

    private:
        T * m_pointer;
    };

    Iterator begin();

    Iterator end();

    Iterator find(const T & value);

    Iterator insert(const Iterator it, const T & value);

    Iterator insert(const Iterator it, int n, const T & value);

    Iterator erase(const Iterator it);

    Iterator erase(const Iterator first, const Iterator last);

private:
    bool is_basic_type() const;

private:
    T * m_data;
    int m_size;
    int m_capacity;
};

template<typename T>
Vector<T>::Vector() : m_data(nullptr), m_size(0), m_capacity(0) {}

template<typename T>
Vector<T>::~Vector()
{
    if (nullptr != m_data)
    {
        delete[] m_data;
        m_data = nullptr;
    }
    m_size = 0;
    m_capacity = 0;
}

template<typename T>
void Vector<T>::push_back(const T & value)
{
    if (m_capacity > m_size)
    {
        m_data[m_size++] = value;
        return;
    }

    if (0 == m_capacity)
        m_capacity = 1;
    else
        m_capacity *= 2;

    T * data = new T[m_capacity];

    for (int i = 0; i < m_size; ++i)
        data[i] = m_data[i];

    if (nullptr != m_data)
    {
        delete[] m_data;
        m_data = nullptr;
    }

    m_data = data;
    m_data[m_size++] = value;
}

template<typename T>
void Vector<T>::pop_back()
{
    if (empty())
        throw std::logic_error("vector is empty");

    --m_size;
}

template<typename T>
int Vector<T>::size() const
{
    return m_size;
}

template<typename T>
int Vector<T>::capacity() const
{
    return m_capacity;
}

template<typename T>
void Vector<T>::show() const
{
    std::cout << "size=" << size() << ", capacity=" << capacity() << std::endl;

    for (int i = 0; i < m_size; ++i)
        std::cout << m_data[i] << " ";

    std::cout << std::endl;
}

template<typename T>
void Vector<T>::reserve(int size)
{
    if (m_capacity >= size)
        return;

    if (0 == m_capacity)
        m_capacity = 1;

    while (m_capacity < size)
        m_capacity *= 2;

    T * data = new T[m_capacity];

    for (int i = 0; i < m_size; ++i)
        data[i] = m_data[i];

    if (nullptr != m_data)
    {
        delete[] m_data;
        m_data = nullptr;
    }

    m_data = data;
}

template<typename T>
void Vector<T>::resize(int size)
{
    if (m_size >= size)
    {
        m_size = size;
        return;
    }

    if (m_capacity >= size)
    {
        for (int i = m_size; i < size; ++i)
            m_data[i] = T();

        m_size = size;
        return;
    }

    if (0 == m_capacity)
        m_capacity = 1;

    while (m_capacity < size)
        m_capacity *= 2;

    T * data = new T[m_capacity];

    for (int i = 0; i < m_size; ++i)
        data[i] = m_data[i];

    for (int i = m_size; i < size; ++i)
        data[i] = T();

    if (nullptr != m_data)
    {
        delete[] m_data;
        m_data = nullptr;
    }

    m_data = data;
    m_size = size;
}

template<typename T>
T & Vector<T>::at(int index)
{
    if (0 > index || index >= m_size)
        throw std::logic_error("out of range");

    return m_data[index];
}

template<typename T>
T & Vector<T>::operator[](int index)
{
    return at(index);
}

template<typename T>
bool Vector<T>::empty() const
{
    return 0 >= m_size;
}

template<typename T>
void Vector<T>::clear()
{
    m_size = 0;
}

template<typename T>
T & Vector<T>::front()
{
    if (empty())
        throw std::logic_error("vector is empty");

    return m_data[0];
}

template<typename T>
T & Vector<T>::back()
{
    if (empty())
        throw std::logic_error("vector is empty");

    return m_data[m_size - 1];
}

template<typename T>
void Vector<T>::swap(Vector<T> & other)
{
    T * data = m_data;
    int size = m_size;
    int capacity = m_capacity;

    m_data = other.m_data;
    m_size = other.m_size;
    m_capacity = other.m_capacity;

    other.m_data = data;
    other.m_size = size;
    other.m_capacity = capacity;
}

template<typename T>
T * Vector<T>::data()
{
    return m_data;
}

template<typename T>
Vector<T> & Vector<T>::operator=(const Vector<T> & other)
{
    if (m_capacity < other.m_size)
    {
        if (nullptr != m_data)
        {
            delete[] m_data;
            m_data = nullptr;
        }

        if (0 == m_capacity)
            m_capacity = 1;

        while (m_capacity < other.m_size)
            m_capacity *= 2;

        m_data = new T[m_capacity];
    }

    for (int i = 0; i < other.m_size; ++i)
        m_data[i] = other.m_data[i];

    m_size = other.m_size;
    return *this;
}

template<typename T>
Vector<T>::Iterator::Iterator() : m_pointer(nullptr) {}

template<typename T>
Vector<T>::Iterator::Iterator(T * pointer) : m_pointer(pointer) {}

template<typename T>
bool Vector<T>::Iterator::operator==(const Iterator other) const
{
    return m_pointer == other.m_pointer;
}

template<typename T>
bool Vector<T>::Iterator::operator!=(const Iterator other) const
{
    return m_pointer != other.m_pointer;
}

template<typename T>
typename Vector<T>::Iterator & Vector<T>::Iterator::operator=(const Iterator other) const
{
    m_pointer = other.m_pointer;
    return *this;
}

template<typename T>
typename Vector<T>::Iterator & Vector<T>::Iterator::operator++()
{
    ++m_pointer;
    return *this;
}

template<typename T>
typename Vector<T>::Iterator Vector<T>::Iterator::operator++(int)
{
    Iterator it = *this;
    ++(*this);
    return it;
}

template<typename T>
typename Vector<T>::Iterator Vector<T>::Iterator::operator+(int i) const
{
    return Iterator(m_pointer + i);
}

template<typename T>
typename Vector<T>::Iterator & Vector<T>::Iterator::operator+=(int i)
{
    m_pointer += i;
    return *this;
}

template<typename T>
typename Vector<T>::Iterator Vector<T>::Iterator::operator-(int i) const
{
    return Iterator(m_pointer - i);
}

template<typename T>
typename Vector<T>::Iterator & Vector<T>::Iterator::operator-=(int i)
{
    m_pointer -= i;
    return *this;
}

template<typename T>
int Vector<T>::Iterator::operator-(const Iterator other) const
{
    return m_pointer - other.m_pointer;
}

template<typename T>
T & Vector<T>::Iterator::operator*()
{
    return *m_pointer;
}

template<typename T>
T * Vector<T>::Iterator::operator->()
{
    return m_pointer;
}

template<typename T>
typename Vector<T>::Iterator Vector<T>::begin()
{
    return Iterator(m_data);
}

template<typename T>
typename Vector<T>::Iterator Vector<T>::end()
{
    return Iterator(m_data + m_size);
}

template<typename T>
typename Vector<T>::Iterator Vector<T>::find(const T & value)
{
    for (Vector<T>::Iterator it = begin(); it != end(); ++it)
    {
        if (*it == value)
            return it;
    }

    return end();
}

template<typename T>
typename Vector<T>::Iterator Vector<T>::insert(const Iterator it, const T & value)
{
    return insert(it, 1, value);
}

template<typename T>
typename Vector<T>::Iterator Vector<T>::insert(const Iterator it, int n, const T & value)
{
    int size = it - begin();

    if (m_capacity >= m_size + n)
    {
        if (is_basic_type())
            std::memmove(m_data + size + n, m_data + size, (m_size - size) * sizeof(T));
        else
        {
            for (int i = m_size - 1; i >= size; --i)
                m_data[i + n] = m_data[i];
        }

        for (int i = 0; i < n; ++i)
            m_data[size + i] = value;

        m_size += n;
        return Iterator(m_data + size);
    }

    if (0 == m_capacity)
        m_capacity = 1;

    while (m_capacity < m_size + n)
        m_capacity *= 2;

    T * data = new T[m_capacity];

    if (is_basic_type())
    {
        std::memcpy(data, m_data, size * sizeof(T));
        std::memcpy(data + size + n, m_data + size, (m_size - size) * sizeof(T));
    } else
    {
        for (int i = 0; i < size; ++i)
            data[i] = m_data[i];

        for (int i = size; i < m_size; ++i)
            data[size + n + i] = m_data[i];
    }

    for (int i = 0; i < n; ++i)
        data[size + i] = value;

    if (nullptr != m_data)
    {
        delete[] m_data;
        m_data = nullptr;
    }

    m_data = data;
    m_size += n;
    return Iterator(m_data + size);
}

template<typename T>
bool Vector<T>::is_basic_type() const
{
    if (std::is_pointer<T>::value)
        return true;

    return (typeid(T) == typeid(bool)) ||
           (typeid(T) == typeid(char)) ||
           (typeid(T) == typeid(unsigned char)) ||
           (typeid(T) == typeid(int)) ||
           (typeid(T) == typeid(unsigned int)) ||
           (typeid(T) == typeid(long)) ||
           (typeid(T) == typeid(unsigned long)) ||
           (typeid(T) == typeid(long long)) ||
           (typeid(T) == typeid(unsigned long long)) ||
           (typeid(T) == typeid(short)) ||
           (typeid(T) == typeid(unsigned short)) ||
           (typeid(T) == typeid(float)) ||
           (typeid(T) == typeid(double));
}

template<typename T>
typename Vector<T>::Iterator Vector<T>::erase(const Iterator it)
{
    if (1 == end() - it)
    {
        --m_size;
        return end();
    }

    int size = it - begin();

    if (is_basic_type())
        std::memmove(m_data + size, m_data + size + 1, (m_size - size - 1) * sizeof(T));
    else
    {
        for (int i = size; i < m_size - 1; ++i)
            m_data[i] = m_data[i + 1];
    }
    --m_size;

    return it;
}

template<typename T>
typename Vector<T>::Iterator Vector<T>::erase(const Iterator first, const Iterator last)
{
    int f = first - begin();
    int l = last - begin();
    int n = last - first;

    if (is_basic_type())
        std::memmove(m_data + f, m_data + l, (m_size - 1) * sizeof(T));
    else
    {
        for (int i = 0; i < m_size - l; ++i)
            m_data[f + i] = m_data[l + i];
    }

    m_size -= n;
    return first;
}

}
}