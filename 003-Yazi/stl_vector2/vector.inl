
template<typename T>
Vector<T>::Vector() : m_data(nullptr), m_size(0), m_capacity(0)
{
}

template<typename T>
Vector<T>::~Vector()
{
    if (m_data != nullptr)
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
    if (m_size < m_capacity)
    {
        m_data[m_size] = value;
        m_size++;
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

    if (is_basic_type())
    {
        std::memcpy(data, m_data, m_size * sizeof(T));
    } else
    {
        for (int i = 0; i < m_size; ++i)
        {
            data[i] = m_data[i];
        }
    }


    if (nullptr != m_data)
    {
        delete[] m_data;
        m_data = nullptr;
    }

    m_data = data;
    m_data[m_size] = value;
    ++m_size;
}

template<typename T>
void Vector<T>::pop_back()
{
    if (0 < m_size)
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
        std::cout << m_data[i] << (m_size - 1 != i ? "," : "");
    }
    std::cout << std::endl;
}





template<typename T>
T & Vector<T>::at(const int index)
{
    if (index < 0 || index >= m_size)
    {
        throw std::logic_error("out of range");
    }
    return m_data[index];
}

template<typename T>
const T & Vector<T>::at(const int index) const
{
    return at(index);
}





template<typename T>
T & Vector<T>::front()
{
    if (0 >= m_size)
    {
        throw std::logic_error("vector is empty");
    }
    return m_data[0];
}

template<typename T>
const T & Vector<T>::front() const
{
    return front();
}





template<typename T>
T & Vector<T>::back()
{
    if (0 >= m_size)
    {
        throw std::logic_error("vector is empty");
    }
    return m_data[m_size - 1];
}

template<typename T>
const T & Vector<T>::back() const
{
    return back();
}





template<typename T>
bool Vector<T>::empty() const noexcept
{
    return 0 == m_size;
}

template<typename T>
void Vector<T>::clear() noexcept
{
    m_size = 0;
}

template<typename T>
int Vector<T>::size() const noexcept
{
    return m_size;
}

template<typename T>
int Vector<T>::capacity() const noexcept
{
    return m_capacity;
}





template<typename T>
T * Vector<T>::data() noexcept
{
    return m_data;
}

template<typename T>
const T * Vector<T>::data() const noexcept
{
    return data();
}





template<typename T>
T & Vector<T>::operator[](const int index)
{
    return at(index);
}

template<typename T>
const T & Vector<T>::operator[](const int index) const
{
    return at(index);
}





template<typename T>
Vector<T> & Vector<T>::operator=(const Vector<T> & other)
{
    if (other.m_size > m_capacity)
    {
        if (m_data != nullptr)
        {
            delete[] m_data;
            m_data = nullptr;
            m_size = 0;
            m_capacity = 0;
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

    if (is_basic_type())
    {
        std::memcpy(m_data, other.m_data, other.m_size * sizeof(T));
    } else
    {
        for (int i = 0; i < other.m_size; ++i)
        {
            m_data[i] = other.m_data[i];
        }
    }

    m_size = other.m_size;
    return *this;
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
void Vector<T>::resize(const int n)
{
    if (n < m_size)
    {
        m_size = n;
        return;
    }
    if (n <= m_capacity)
    {
        for (int i = m_size; i < n; ++i)
        {
            m_data[i] = T();
        }
        m_size = n;
        return;
    }
    while (n > m_capacity)
    {
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
            std::memcpy(data, m_data, m_size * sizeof(T));
        } else
        {
            for (int i = 0; i < m_size; ++i)
            {
                data[i] = m_data[i];
            }
        }

        for (int i = m_size; i < n; ++i)
        {
            data[i] = T();
        }
        if (nullptr != m_data)
        {
            delete[] m_data;
            m_data = nullptr;
        }
        m_data = data;
        m_size = n;
    }
}

template<typename T>
void Vector<T>::resize(const int n, const T & value)
{
    if (n < m_size)
    {
        m_size = n;
        return;
    }
    if (n <= m_capacity)
    {
        for (int i = m_size; i < n; ++i)
        {
            m_data[i] = value;
        }
        m_size = n;
        return;
    }
    while (n > m_capacity)
    {
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
            std::memcpy(data, m_data, m_size * sizeof(T));
        } else
        {
            for (int i = 0; i < m_size; ++i)
            {
                data[i] = m_data[i];
            }
        }

        for (int i = m_size; i < n; ++i)
        {
            data[i] = value;
        }
        if (nullptr != m_data)
        {
            delete[] m_data;
            m_data = nullptr;
        }
        m_data = data;
        m_size = n;
    }
}





template<typename T>
void Vector<T>::reserve(const int n)
{
    if (n <= m_capacity)
    {
        return;
    }

    while (n > m_capacity)
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
        std::memcpy(data, m_data, m_size * sizeof(T));
    } else
    {
        for (int i = 0; i < m_size; ++i)
        {
            data[i] = m_data[i];
        }
    }

    if (nullptr != m_data)
    {
        delete[] m_data;
        m_data = nullptr;
    }
    m_data = data;
}





template<typename T>
Vector<T>::iterator::iterator() : m_pointer(nullptr) {}

template<typename T>
Vector<T>::iterator::iterator(T * pointer) : m_pointer(pointer) {}

template<typename T>
Vector<T>::iterator::~iterator() {}



template<typename T>
bool Vector<T>::iterator::operator==(const iterator & other) const
{
    return other.m_pointer == m_pointer;
}

template<typename T>
bool Vector<T>::iterator::operator!=(const iterator & other) const
{
    return other.m_pointer != m_pointer;
}



template<typename T>
typename Vector<T>::iterator & Vector<T>::iterator::operator=(const iterator & other)
{
    m_pointer = other.m_pointer;
    return *this;
}



template<typename T>
typename Vector<T>::iterator & Vector<T>::iterator::operator++()
{
    m_pointer += 1;
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
typename Vector<T>::iterator Vector<T>::iterator::operator+(int i)
{
    iterator it = *this;
    it.m_pointer += i;
    return it;
}

template<typename T>
typename Vector<T>::iterator & Vector<T>::iterator::operator+=(int i)
{
    m_pointer += i;
    return *this;
}



template<typename T>
typename Vector<T>::iterator & Vector<T>::iterator::operator--()
{
    m_pointer -= 1;
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
typename Vector<T>::iterator Vector<T>::iterator::operator-(int i)
{
    iterator it = *this;
    it.m_pointer -= i;
    return it;
}

template<typename T>
typename Vector<T>::iterator & Vector<T>::iterator::operator-=(int i)
{
    m_pointer -= i;
    return *this;
}



template<typename T>
int Vector<T>::iterator::operator-(const iterator & other) const
{
    return m_pointer - other.m_pointer;
}



template<typename T>
T & Vector<T>::iterator::operator*()
{
    return *m_pointer;
}

template<typename T>
T * Vector<T>::iterator::operator->()
{
    return m_pointer;
}





template<typename T>
typename Vector<T>::iterator Vector<T>::begin() noexcept
{
    iterator it(m_data);
    return it;
}

template<typename T>
typename Vector<T>::iterator Vector<T>::end() noexcept
{
    iterator it(m_data + m_size);
    return it;
}





template<typename T>
Vector<T>::reverse_iterator::reverse_iterator() : m_pointer(nullptr) {}

template<typename T>
Vector<T>::reverse_iterator::reverse_iterator(T * pointer) : m_pointer(pointer) {}



template<typename T>
bool Vector<T>::reverse_iterator::operator==(const reverse_iterator & other) const
{
    return other.m_pointer == m_pointer;
}

template<typename T>
bool Vector<T>::reverse_iterator::operator!=(const reverse_iterator & other) const
{
    return other.m_pointer != m_pointer;
}

template<typename T>
typename Vector<T>::reverse_iterator & Vector<T>::reverse_iterator::operator=(const reverse_iterator & other)
{
    m_pointer = other.m_pointer;
    return *this;
}



template<typename T>
typename Vector<T>::reverse_iterator & Vector<T>::reverse_iterator::operator++()
{
    m_pointer -= 1;
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
typename Vector<T>::reverse_iterator Vector<T>::reverse_iterator::operator+(int i)
{
    reverse_iterator it = *this;
    it.m_pointer -= i;
    return it;
}

template<typename T>
typename Vector<T>::reverse_iterator & Vector<T>::reverse_iterator::operator+=(int i)
{
    m_pointer -= i;
    return *this;
}



template<typename T>
typename Vector<T>::reverse_iterator & Vector<T>::reverse_iterator::operator--()
{
    m_pointer += 1;
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
typename Vector<T>::reverse_iterator Vector<T>::reverse_iterator::operator-(int i)
{
    reverse_iterator it = *this;
    it.m_pointer += i;
    return it;
}

template<typename T>
typename Vector<T>::reverse_iterator & Vector<T>::reverse_iterator::operator-=(int i)
{
    m_pointer += i;
    return *this;
}



template<typename T>
T & Vector<T>::reverse_iterator::operator*()
{
    return *m_pointer;
}

template<typename T>
T * Vector<T>::reverse_iterator::operator->()
{
    return m_pointer;
}





template<typename T>
typename Vector<T>::reverse_iterator Vector<T>::rbegin() noexcept
{
    reverse_iterator it(m_data + m_size - 1);
    return it;
}

template<typename T>
typename Vector<T>::reverse_iterator Vector<T>::rend() noexcept
{
    Vector<T>::reverse_iterator it(m_data - 1);
    return it;
}





template<typename T>
typename Vector<T>::iterator Vector<T>::insert(iterator pos, const T & value)
{
    return insert(pos, 1, value);
}

template<typename T>
typename Vector<T>::iterator Vector<T>::insert(iterator pos, int n, const T & value)
{
    int size = pos - begin();
    if (m_size + n <= m_capacity)
    {
        if (is_basic_type())
        {
            std::memmove(m_data + size + n, m_data + size, (m_size - size) * sizeof(T));
        } else
        {
            for (int i = m_size; i > size; --i)
            {
                m_data[i + n - 1] = m_data[i - 1];
            }
        }

        for (int i = 0; i < n; ++i)
        {
            m_data[size + i] = value;
        }

        m_size += n;
        return iterator(m_data + size);
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
        std::memcpy(data, m_data, size * sizeof(T));
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
        std::memcpy(data + size + n, m_data + size, (m_size - size) * sizeof(T));
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

    return iterator(m_data + size);
}





template<typename T>
typename Vector<T>::iterator Vector<T>::erase(iterator pos)
{
    if (pos == end())
    {
        throw std::logic_error("out of range");
    }

    if (end() - pos == 1)
    {
        m_size -= 1;
        return end();
    }
    int size = pos - begin();

    if (is_basic_type())
    {
        std::memmove(m_data + size, m_data + size + 1, (m_size - size - 1) * sizeof(T));
    } else
    {
        for (int i = size; i < m_size - 1; ++i)
        {
            m_data[i] = m_data[i + 1];
        }
    }

    m_size -= 1;
    return pos;
}

template<typename T>
typename Vector<T>::iterator Vector<T>::erase(iterator first, iterator last)
{
    int f = first - begin();
    int l = last - begin();
    int n = last - first;

    if (is_basic_type())
    {
        std::memmove(m_data + f, m_data + l, (m_size - l) * sizeof(T));
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



