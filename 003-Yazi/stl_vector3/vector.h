#pragma once

#include <typeinfo>
#include <cstring>

template<typename T>
class Vector
{
public:
    Vector() = default;
    ~Vector();

    bool is_basic_type() const;

    void push_back(const T & value);
    void pop_back();

    void show() const;
    int size() const noexcept { return m_size; }
    int capacity() const noexcept { return m_capacity; }

    void clear() noexcept { m_size = 0; }
    bool empty() const noexcept { return m_size <= 0; }
    bool range(const int index) { return index >= 0 && index < m_size; }

    T & at(const int index);
    const T & at(const int index) const;
    T & operator[](const int index) { return at(index); }
    const T & operator[](const int index) const { return at(index); }
    T & front() { return at(0); }
    const T & front() const { return at(0); }
    T & back() { return at(m_size - 1); }
    const T & back() const { return at(m_size - 1); }

    T * data() noexcept { return m_data; }
    const T * data() const noexcept { return m_data; }
    void swap(Vector & other);

    void reserve(const int n);
    void resize(const int n, const T & value = T());
    T & operator=(const T & other);

    class iterator
    {
    public:
        iterator() = default;
        ~iterator() = default;
        iterator(T * ptr) : m_ptr(ptr) {}

        iterator & operator=(const iterator & other);
        bool operator==(const iterator & other) const { return m_ptr == other.m_ptr; }
        bool operator!=(const iterator & other) const { return m_ptr != other.m_ptr; }

        iterator & operator++();
        iterator operator++(int);
        iterator operator+(const int i);
        iterator & operator+=(const int i);

        iterator & operator--();
        iterator operator--(int);
        iterator operator-(const int i);
        iterator & operator-=(const int i);

        int operator-(const iterator & other) const;

        T * operator->() { return m_ptr; }
        T & operator*() { return *m_ptr; }

    private:
        T * m_ptr = nullptr;
    };

    iterator begin() noexcept { return iterator(m_data); }
    iterator end() noexcept { return iterator(m_data + m_size); }

    class reverse_iterator
    {
    public:
        reverse_iterator() = default;
        ~reverse_iterator() = default;
        reverse_iterator(T * ptr) : m_ptr(ptr) {}

        reverse_iterator & operator=(const reverse_iterator & other);
        bool operator==(const reverse_iterator & other) const { return m_ptr == other.m_ptr; }
        bool operator!=(const reverse_iterator & other) const { return m_ptr != other.m_ptr; }

        reverse_iterator & operator++();
        reverse_iterator operator++(int);
        reverse_iterator operator+(const int i);
        reverse_iterator & operator+=(const int i);

        reverse_iterator & operator--();
        reverse_iterator operator--(int);
        reverse_iterator operator-(const int i);
        reverse_iterator & operator-=(const int i);

        T * operator->() { return m_ptr; }
        T & operator*() { return *m_ptr; }
    private:
        T * m_ptr = nullptr;
    };

    reverse_iterator rbegin() noexcept { return reverse_iterator(m_data + m_size - 1); }
    reverse_iterator rend() noexcept { return reverse_iterator(m_data - 1); }

    iterator insert(iterator it, const T & value);
    iterator insert(iterator it, const int n, const T & value);

    iterator erase(iterator it);
    iterator erase(iterator first, iterator last);

private:
    T * m_data = nullptr;
    int m_size = 0;
    int m_capacity = 0;
};

#include <utilities/vector.inl>