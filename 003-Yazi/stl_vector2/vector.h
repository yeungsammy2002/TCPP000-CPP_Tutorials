
#pragma once

#include <stdexcept>
#include <cstring>
#include <typeinfo>

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
    void show() const;

    T & at(const int index);
    const T & at(const int index) const;

    T & front();
    const T & front() const;

    T & back();
    const T & back() const;

    bool empty() const noexcept;
    void clear() noexcept;
    int size() const noexcept;
    int capacity() const noexcept;

    T * data() noexcept;
    const T * data() const noexcept;

    T & operator[](const int index);
    const T & operator[](const int index) const;

    Vector<T> & operator=(const Vector<T> & other);

    void swap(Vector<T> & other);

    void resize(const int n);
    void resize(const int n, const T & val);

    void reserve(const int n);

    class iterator
    {
    public:
        iterator();
        iterator(T * pointer);
        ~iterator();

        bool operator==(const iterator & other) const;
        bool operator!=(const iterator & other) const;

        iterator & operator=(const iterator & other);

        iterator & operator++();
        iterator operator++(int);

        iterator operator+(int i);
        Vector<T>::iterator & operator+=(int i);

        iterator & operator--();
        iterator operator--(int);

        iterator operator-(int i);
        iterator & operator-=(int i);

        int operator-(const iterator & other) const;

        T & operator*();
        T * operator->();

    private:
        T * m_pointer;
    };

    iterator begin() noexcept;
    iterator end() noexcept;

    class reverse_iterator
    {
    public:
        reverse_iterator();
        reverse_iterator(T * pointer);
        ~reverse_iterator() = default;

        bool operator==(const reverse_iterator & other) const;
        bool operator!=(const reverse_iterator & other) const;

        reverse_iterator & operator=(const reverse_iterator & other);

        reverse_iterator & operator++();
        reverse_iterator operator++(int);

        reverse_iterator operator+(int i);
        reverse_iterator & operator+=(int i);

        reverse_iterator & operator--();
        reverse_iterator operator--(int);

        reverse_iterator operator-(int i);
        reverse_iterator & operator-=(int i);

        T & operator*();
        T * operator->();

    private:
        T * m_pointer;
    };

    reverse_iterator rbegin() noexcept;
    reverse_iterator rend() noexcept;

    iterator insert(iterator pos, const T & value);
    iterator insert(iterator pos, int n, const T & value);

    iterator erase(iterator pos);
    iterator erase(iterator first, iterator last);

    bool is_basic_type() const;

private:
    T * m_data;
    int m_size;
    int m_capacity;
};

#include <stl/vector.inl>

}
}