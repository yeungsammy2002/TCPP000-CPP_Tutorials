template<typename T>
Vector<T>::Vector() : m_data(nullptr), m_size(0), m_capacity(0) {}

template<typename T>
void Vector<T>::push_back(const T & value)
{
    if (m_capacity > m_size)
    {
        m_data[m_size++] = value;
        return;
    }
    if (0 == m_capacity)
    {
        m_capacity = 1;
    } else
    {
        m_capacity *= 2;
    }
    T * data = new T[m_capacity];
    for (int i = 0; i < m_size; ++i)
    {
        data[i] = m_data[i];
    }
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
    std::cout << "[";
    for (int i = 0; i < m_size; ++i)
    {
        if (0 != i)
        {
            std::cout << ",";
        }
        std::cout << m_data[i];
    }
    std::cout << "]";
    std::cout << std::endl;
}

template<typename T>
void Vector<T>::reserve(const int size)
{
    if (size <= m_capacity)
    {
        return;
    }
    while (size > m_capacity)
    {
        if (0 == m_capacity)
        {
            m_capacity = 1;
        } else
        {
            m_capacity *= 2;
        }
    }
    T * data = new T[m_capacity];
    for (int i = 0; i < m_size; ++i)
    {
        data[i] = m_data[i];
    }
    if (nullptr != m_data)
    {
        delete[] m_data;
        m_data = nullptr;
    }
    m_data = data;
}

template<typename T>
void Vector<T>::resize(const int size)
{
    if (size <= m_size)
    {
        m_size = size;
        return;
    }
    if (size <= m_capacity)
    {
        for (int i = m_size; i < size; ++i)
        {
            m_data[i] = T();
        }
        m_size = size;
        return;
    }
    while (size > m_capacity)
    {
        if (0 == m_capacity)
        {
            m_capacity = 1;
        } else
        {
            m_capacity *= 2;
        }
    }
    T * data = new T[m_capacity];
    for (int i = 0; i < m_size; ++i)
    {
        data[i] = m_data[i];
    }
    for (int i = m_size; i < size; ++i)
    {
        data[i] = T();
    }
    if (nullptr != m_data)
    {
        delete[] m_data;
        m_data = nullptr;
    }
    m_data = data;
    m_size = size;
}

template<typename T>
T & Vector<T>::at(const int index)
{
    if (index < 0 || index >= size())
    {
        throw std::logic_error("out of range");
    }
    return m_data[index];
}

template<typename T>
T & Vector<T>::operator[](const int index)
{
    return at(index);
}

template<typename T>
void Vector<T>::clear()
{
    m_size = 0;
}

template<typename T>
bool Vector<T>::empty() const
{
    return 0 >= m_size;
}

template<typename T>
T & Vector<T>::front()
{
    if (empty())
    {
        throw std::logic_error("vector is empty");
    }
    return m_data[0];
}

template<typename T>
T & Vector<T>::back()
{
    if (empty())
    {
        throw std::logic_error("vector is empty");
    }
    return m_data[m_size - 1];
}

template<typename T>
void Vector<T>::swap(Vector<T> & other)
{
    T * data = other.m_data;
    int size = other.m_size;
    int capacity = other.m_capacity;

    other.m_data = m_data;
    other.m_size = m_size;
    other.m_capacity = m_capacity;

    m_data = data;
    m_size = size;
    m_capacity = capacity;
}

template<typename T>
Vector<T> & Vector<T>::operator=(const Vector<T> & other)
{
    if (other.m_size > m_capacity)
    {
        if (nullptr != m_data)
        {
            delete[] m_data;
            m_data = nullptr;
        }
        while (other.m_size > m_capacity)
        {
            if (0 == m_capacity)
            {
                m_capacity = 1;
            } else
            {
                m_capacity *= 2;
            }
        }
        m_data = new T[m_capacity];
    }
    for (int i = 0; i < other.m_size; ++i)
    {
        m_data[i] = other.m_data[i];
    }
    m_size = other.m_size;
    return *this;
}

template<typename T>
T * Vector<T>::data()
{
    return m_data;
}

template<typename T>
Vector<T>::Iterator::Iterator() : m_pointer(nullptr) {}

template<typename T>
Vector<T>::Iterator::Iterator(T * pointer) : m_pointer(pointer) {}

template<typename T>
typename Vector<T>::Iterator & Vector<T>::Iterator::operator++()
{
    m_pointer += 1;
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
bool Vector<T>::Iterator::operator==(const Iterator other) const
{
    return other.m_pointer == m_pointer;
}

template<typename T>
bool Vector<T>::Iterator::operator!=(const Iterator other) const
{
    return !(other == *this);
}

template<typename T>
typename Vector<T>::Iterator Vector<T>::Iterator::operator+(const int i) const
{
    return Iterator(m_pointer + i);
}

template<typename T>
typename Vector<T>::Iterator Vector<T>::Iterator::operator-(const int i) const
{
    return Iterator(m_pointer - i);
}

template<typename T>
typename Vector<T>::Iterator & Vector<T>::Iterator::operator+=(const int i) const
{
    m_pointer += i;
    return *this;
}

template<typename T>
typename Vector<T>::Iterator & Vector<T>::Iterator::operator-=(const int i) const
{
    m_pointer -= i;
    return *this;
}

template<typename T>
int Vector<T>::Iterator::operator-(const Iterator & other) const
{
    return m_pointer - other.m_pointer;
}

template<typename T>
T * Vector<T>::Iterator::operator->()
{
    return m_pointer;
}

template<typename T>
T & Vector<T>::Iterator::operator*()
{
    return *m_pointer;
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
    for (auto it = begin(); it != end(); ++it)
    {
        if (value == *it)
        {
            return it;
        }
    }
    return end();
}

template<typename T>
typename Vector<T>::Iterator Vector<T>::insert(Iterator it, const T & value)
{
    return insert(it, 1, value);
}

template<typename T>
typename Vector<T>::Iterator Vector<T>::insert(Iterator it, int n, const T & value)
{
    int size = it - begin();
    if (m_size + n <= m_capacity)
    {
        if (is_basic_type())
        {
            std::memmove(m_data + size + n, m_data + size, sizeof(T) * (m_size - size));
        } else
        {
            for (int i = m_size - 1; i >= size; --i)
            {
                m_data[i + n] = m_data[i];
            }
        }
        for (int i = 0; i < n; ++i)
        {
            m_data[size + i] = value;
        }
        m_size += n;
        return Iterator(m_data + size);
    }
    while (m_size + n > m_capacity)
    {
        if (0 == m_capacity)
        {
            m_capacity = 1;
        } else
        {
            m_capacity *= 2;
        }
    }
    T * data = new T[m_capacity];
    if (is_basic_type())
    {
        std::memcpy(data, m_data, sizeof(T) * size);
    } else
    {
        for (int i = 0; i < size; ++i)
        {
            data[i] = m_data[i];
        }
    }
    for (int i = 0; i < n; ++i)
    {
        data[size + i] = value;
    }
    if (is_basic_type())
    {
        std::memcpy(data + size + n, m_data + size, sizeof(T) * (m_size - size));
    } else
    {
        for (int i = size; i < m_size; ++i)
        {
            data[i + n] = m_data[i];
        }
    }
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
//    return false;
    return (typeid(T) == typeid(bool)) ||
           (typeid(T) == typeid(char)) ||
           (typeid(T) == typeid(unsigned char)) ||
           (typeid(T) == typeid(short)) ||
           (typeid(T) == typeid(unsigned short)) ||
           (typeid(T) == typeid(int)) ||
           (typeid(T) == typeid(unsigned int)) ||
           (typeid(T) == typeid(long)) ||
           (typeid(T) == typeid(unsigned long)) ||
           (typeid(T) == typeid(float)) ||
           (typeid(T) == typeid(double));
}

template<typename T>
typename Vector<T>::Iterator Vector<T>::erase(Iterator it)
{
    const int size = it - begin();
    if (size < 0 || size >= m_size)
    {
        return Iterator(m_data + size);
    }
    if (is_basic_type() && size != m_size - 1)
    {
        std::memmove(m_data + size, m_data + size + 1, sizeof(T) * (m_size - size - 1));
    } else
    {
        for (int i = size; i < m_size - 1; ++i)
        {
            m_data[i] = m_data[i + 1];
        }
    }
    --m_size;
    return Iterator(m_data + size);
}

template<typename T>
typename Vector<T>::Iterator Vector<T>::erase(Iterator first, Iterator last)
{
    int f = first - begin();
    int l = last - begin();
    int n = l - f;

    if (is_basic_type())
    {
        std::memmove(m_data + f, m_data + l, sizeof(T) * (m_size - l));
    } else
    {
        for (int i = 0; i < m_size - l; ++i)
        {
            m_data[f + i] = m_data[l + i];
        }
    }

    m_size -= n;
    return Iterator(m_data + f);
}