template<typename T>
Vector<T>::~Vector()
{
    if (m_data != nullptr)
    {
        delete[] m_data;
    }
    m_data = nullptr;
    m_size = 0;
    m_capacity = 0;
}





template<typename T>
bool Vector<T>::is_basic_type() const
{
    if (std::is_pointer<T>::value)
    {
        return true;
    }
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
void Vector<T>::push_back(const T & value)
{
    if (m_capacity > m_size)
    {
        m_data[m_size++] = value;
        return;
    }

    if (m_capacity == 0)
    {
        m_capacity = 1;
    } else
    {
        m_capacity *= 2;
    }

    T * data = new T[m_capacity];

    if (is_basic_type())
    {
        std::memcpy(data, m_data, sizeof(T) * m_size);
    } else
    {
        for (int i = 0; i < m_size; ++i)
        {
            data[i] = m_data[i];
        }
    }

    if (m_data != nullptr)
    {
        delete[] m_data;
    }

    m_data = data;
    m_data[m_size++] = value;
}





template<typename T>
void Vector<T>::pop_back()
{
    if (m_size > 0)
    {
        --m_size;
    }
}





template<typename T>
void Vector<T>::show() const
{
    std::cout << "size=" << m_size << ", capacity=" << m_capacity << std::endl;
    for (int i = 0; i < m_size; ++i)
    {
        std::cout << m_data[i] << (i != m_size - 1 ? "," : "");
    }
    std::cout << std::endl;
}





template<typename T>
T & Vector<T>::at(const int index)
{
    if (empty() || !range(index))
    {
        throw std::logic_error("vector is empty or index is out of range");
    }
    return m_data[index];
}

template<typename T>
const T & Vector<T>::at(const int index) const
{
    if (empty() || !range(index))
    {
        throw std::logic_error("vector is empty is index is out of range");
    }
    return m_data[index];
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
void Vector<T>::reserve(const int n)
{
    if (m_capacity >= n)
    {
        return;
    }

    while (m_capacity < n)
    {
        if (m_capacity == 0)
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
        std::memcpy(data, m_data, sizeof(T) * m_size);
    } else
    {
        for (int i = 0; i < m_size; ++i)
        {
            data[i] = m_data[i];
        }
    }

    if (m_data != nullptr)
    {
        delete[] m_data;
    }

    m_data = data;
}

template<typename T>
void Vector<T>::resize(const int n, const T & value)
{
    if (m_size >= n)
    {
        m_size = n;
        return;
    }

    if (m_capacity < n)
    {
        while (m_capacity < n)
        {
            if (m_capacity == 0)
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
            std::memcpy(data, m_data, sizeof(T) * m_size);
        } else
        {
            for (int i = 0; i < m_size; ++i)
            {
                data[i] = m_data[i];
            }
        }

        if (m_data != nullptr)
        {
            delete[] m_data;
        }

        m_data = data;
    }

    for (int i = m_size; i < n; ++i)
    {
        m_data[i] = T();
    }

    m_size = n;
}

template<typename T>
T & Vector<T>::operator=(const T & other)
{
    if (m_capacity < other.m_size)
    {
        if (m_data != nullptr)
        {
            delete[] m_data;
        }
        m_data = nullptr;
        m_size = 0;
        m_capacity = 0;

        while (m_capacity < other.m_size)
        {
            if (m_capacity == 0)
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
typename Vector<T>::iterator & Vector<T>::iterator::operator=(const iterator & other)
{
    m_ptr = other.m_ptr;
    return *this;
}

template<typename T>
typename Vector<T>::iterator & Vector<T>::iterator::operator++()
{
    m_ptr += 1;
    return *this;
}

template<typename T>
typename Vector<T>::iterator Vector<T>::iterator::operator++(int)
{
    iterator it = *this;
    ++(*this);
    return it;
}

template<typename T>
typename Vector<T>::iterator Vector<T>::iterator::operator+(const int i)
{
    return iterator(m_ptr + i);
}

template<typename T>
typename Vector<T>::iterator & Vector<T>::iterator::operator+=(const int i)
{
    m_ptr += i;
    return *this;
}

template<typename T>
typename Vector<T>::iterator & Vector<T>::iterator::operator--()
{
    m_ptr -= 1;
    return *this;
}

template<typename T>
typename Vector<T>::iterator Vector<T>::iterator::operator--(int)
{
    iterator it = *this;
    --(*this);
    return it;
}

template<typename T>
typename Vector<T>::iterator Vector<T>::iterator::operator-(const int i)
{
    return iterator(m_ptr - i);
}

template<typename T>
typename Vector<T>::iterator & Vector<T>::iterator::operator-=(const int i)
{
    m_ptr -= i;
    return *this;
}

template<typename T>
int Vector<T>::iterator::operator-(const iterator & other) const
{
    return m_ptr - other.m_ptr;
}





template<typename T>
typename Vector<T>::reverse_iterator & Vector<T>::reverse_iterator::operator=(const reverse_iterator & other)
{
    m_ptr = other.m_ptr;
    return *this;
}

template<typename T>
typename Vector<T>::reverse_iterator & Vector<T>::reverse_iterator::operator++()
{
    m_ptr -= 1;
    return *this;
}

template<typename T>
typename Vector<T>::reverse_iterator Vector<T>::reverse_iterator::operator++(int)
{
    reverse_iterator it = *this;
    ++(*this);
    return it;
}

template<typename T>
typename Vector<T>::reverse_iterator Vector<T>::reverse_iterator::operator+(const int i)
{
    return reverse_iterator(m_ptr - i);
}

template<typename T>
typename Vector<T>::reverse_iterator & Vector<T>::reverse_iterator::operator+=(const int i)
{
    m_ptr -= i;
    return *this;
}

template<typename T>
typename Vector<T>::reverse_iterator & Vector<T>::reverse_iterator::operator--()
{
    m_ptr += 1;
    return *this;
}

template<typename T>
typename Vector<T>::reverse_iterator Vector<T>::reverse_iterator::operator--(int)
{
    reverse_iterator it = *this;
    --(*this);
    return it;
}

template<typename T>
typename Vector<T>::reverse_iterator Vector<T>::reverse_iterator::operator-(const int i)
{
    return reverse_iterator(m_ptr + i);
}

template<typename T>
typename Vector<T>::reverse_iterator & Vector<T>::reverse_iterator::operator-=(const int i)
{
    m_ptr += i;
    return *this;
}





template<typename T>
typename Vector<T>::iterator Vector<T>::insert(iterator it, const T & value)
{
    return insert(it, 1, value);
}

template<typename T>
typename Vector<T>::iterator Vector<T>::insert(iterator it, const int n, const T & value)
{
    int pos = it - begin();

    if (m_capacity >= m_size + n)
    {
        if (is_basic_type())
        {
            std::memmove(m_data + pos + n, m_data + pos, sizeof(T) * (m_size - pos));
        } else
        {
            for (int i = m_size - 1; i >= pos; --i)
            {
                m_data[i + n] = m_data[i];
            }
        }
    } else
    {
        while (m_capacity < m_size + n)
        {
            if (m_capacity == 0)
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
            std::memcpy(data, m_data, sizeof(T) * pos);
            std::memcpy(data + pos + n, m_data + pos, sizeof(T) * (m_size - pos));
        } else
        {
            for (int i = 0; i < pos; ++i)
            {
                data[i] = m_data[i];
            }
            for (int i = pos; i < m_size; ++i)
            {
                data[i + n] = m_data[i];
            }
        }

        if (m_data != nullptr)
        {
            delete[] m_data;
            m_data = nullptr;
        }

        m_data = data;
    }

    for (int i = 0; i < n; ++i)
    {
        m_data[pos + i] = value;
    }

    m_size += n;
    return iterator(m_data + pos);
}





template<typename T>
typename Vector<T>::iterator Vector<T>::erase(iterator it)
{
    int pos = it - begin();

    if (!range(pos))
    {
        throw std::logic_error("out of range");
    }

    if (end() - it == 1)
    {
        --m_size;
        return end();
    }

    if (is_basic_type())
    {
        std::memmove(m_data + pos, m_data + pos + 1, sizeof(T) * (m_size - pos - 1));
    } else
    {
        for (int i = pos; i < m_size - 1; ++i)
        {
            m_data[i] = m_data[i + 1];
        }
    }

    --m_size;
    return iterator(m_data + pos);
}

template<typename T>
typename Vector<T>::iterator Vector<T>::erase(iterator first, iterator last)
{
    int f = first - begin();
    int l = last - begin();
    int n = last - first;

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
    return first;
}