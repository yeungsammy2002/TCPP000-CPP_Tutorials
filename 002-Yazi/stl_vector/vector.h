#pragma once

#include <stdexcept>
#include <cstring>

namespace demo {
namespace stl {

template<typename T>
class Vector
{
public:
    Vector();

    ~Vector() = default;

    void push_back(const T & value);

    void pop_back();

    int size() const;

    int capacity() const;

    void show() const;

    void reserve(const int size);

    void resize(const int size);

    T & at(const int index);

    T & operator[](const int index);

    void clear();

    bool empty() const;

    T & front();

    T & back();

    void swap(Vector<T> & other);

    Vector<T> & operator=(const Vector<T> & other);

    T * data();

    class Iterator
    {
    public:
        Iterator();

        Iterator(T * pointer);

        ~Iterator() = default;

        Iterator & operator++();

        Iterator operator++(int);

        bool operator==(const Iterator other) const;

        bool operator!=(const Iterator other) const;

        Iterator operator+(const int i) const;

        Iterator operator-(const int i) const;

        Iterator & operator+=(const int i) const;

        Iterator & operator-=(const int i) const;

        int operator-(const Iterator & other) const;

        T * operator->();

        T & operator*();

    private:
        T * m_pointer;
    };

    Iterator begin();

    Iterator end();

    Iterator find(const T & value);

    Iterator insert(Iterator it, const T & value);

    Iterator insert(Iterator it, int n, const T & value);

    bool is_basic_type() const;

    Iterator erase(Iterator it);

    Iterator erase(Iterator first, Iterator last);

private:
    T * m_data;
    int m_size;
    int m_capacity;
};

#include <stl/vector.inl>

}
}