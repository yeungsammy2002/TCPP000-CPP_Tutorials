
template<typename T>
void DataStream::write(const std::vector<T> & value)
{
    char type = DataType::VECTOR;
    write((char *) &type, sizeof(char));
    int32_t len = value.size();
    write(len);
    for (int i = 0; i < len; ++i)
    {
        write(value[i]);
    }
}

template<typename T>
void DataStream::write(const std::list<T> & value)
{
    char type = DataType::LIST;
    write((char *) &type, sizeof(char));
    int32_t len = value.size();
    write(len);
    for (auto it = value.begin(); it != value.end(); ++it)
    {
        write(*it);
    }
}

template<typename K, typename V>
void DataStream::write(const std::map<K, V> & value)
{
    char type = DataType::MAP;
    write((char *) &type, sizeof(char));
    int32_t len = value.size();
    write(len);
    for (auto it = value.begin(); it != value.end(); ++it)
    {
        write(it->first);
        write(it->second);
    }
}

template<typename T>
void DataStream::write(const std::set<T> & value)
{
    char type = DataType::SET;
    write((char *) &type, sizeof(char));
    int32_t len = value.size();
    write(len);
    for (auto it = value.begin(); it != value.end(); ++it)
    {
        write(*it);
    }
}



template<typename T, typename ...Args>
void DataStream::write_args(const T & head, const Args & ... args)
{
    write(head);
    write_args(args...);
}



template<typename T>
DataStream & DataStream::operator<<(const std::vector<T> & value)
{
    write(value);
    return *this;
}

template<typename T>
DataStream & DataStream::operator<<(const std::list<T> & value)
{
    write(value);
    return *this;
}

template<typename K, typename V>
DataStream & DataStream::operator<<(const std::map<K, V> & value)
{
    write(value);
    return *this;
}

template<typename T>
DataStream & DataStream::operator<<(const std::set<T> & value)
{
    write(value);
    return *this;
}





template<typename T>
bool DataStream::read(std::vector<T> & value)
{
    value.clear();
    if (DataType::VECTOR != m_buf[m_pos])
    {
        return false;
    }
    ++m_pos;
    int len = 0;
    read(len);
    for (int i = 0; i < len; ++i)
    {
        T v;
        read(v);
        value.push_back(v);
    }
    return true;
}

template<typename T>
bool DataStream::read(std::list<T> & value)
{
    value.clear();
    if (DataType::LIST != m_buf[m_pos])
    {
        return false;
    }
    ++m_pos;
    int len = 0;
    read(len);
    for (int i = 0; i < len; ++i)
    {
        T v;
        read(v);
        value.push_back(v);
    }
    return true;
}

template<typename K, typename V>
bool DataStream::read(std::map<K, V> & value)
{
    value.clear();
    if (DataType::MAP != m_buf[m_pos])
    {
        return false;
    }
    ++m_pos;
    int len = 0;
    read(len);
    for (int i = 0; i < len; ++i)
    {
        K k;
        read(k);

        V v;
        read(v);

        value[k] = v;
    }
    return true;
}

template<typename T>
bool DataStream::read(std::set<T> & value)
{
    value.clear();
    if (DataType::SET != m_buf[m_pos])
    {
        return false;
    }
    ++m_pos;
    int len = 0;
    read(len);
    for (int i = 0; i < len; ++i)
    {
        T v;
        read(v);
        value.insert(v);
    }
    return true;
}



template<typename T, typename ...Args>
bool DataStream::read_args(T & head, Args & ...args)
{
    read(head);
    return read_args(args...);
}



template<typename T>
DataStream & DataStream::operator>>(std::vector<T> & value)
{
    read(value);
    return *this;
}

template<typename T>
DataStream & DataStream::operator>>(std::list<T> & value)
{
    read(value);
    return *this;
}

template<typename K, typename V>
DataStream & DataStream::operator>>(std::map<K, V> & value)
{
    read(value);
    return *this;
}

template<typename T>
DataStream & DataStream::operator>>(std::set<T> & value)
{
    read(value);
    return *this;
}


