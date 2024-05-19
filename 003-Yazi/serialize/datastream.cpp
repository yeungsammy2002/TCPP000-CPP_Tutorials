#include <serialize/datastream.h>

using namespace demo::serialize;

Datastream::Datastream() : m_pos(0) {}

void Datastream::reserve(int len)
{
    int size = m_buff.size();
    int capacity = m_buff.capacity();
    if (size + len > capacity)
    {
        while (size + len > capacity)
        {
            if (0 == capacity)
            {
                capacity = 1;
            } else
            {
                capacity *= 2;
            }
        }
    }
    m_buff.reserve(capacity);
}

void Datastream::write(const char * buff, int len)
{
    int size = m_buff.size();
    reserve(len);
    m_buff.resize(size + len);
    std::memcpy(&m_buff[size], buff, len);
}

void Datastream::write(bool value)
{
    char type = (char) DataType::BOOL;
    write(&type, sizeof(char));
    write((char *) &value, sizeof(char));
}

void Datastream::write(char value)
{
    char type = (char) DataType::CHAR;
    write(&type, sizeof(char));
    write(&value, sizeof(char));
}

void Datastream::write(int32_t value)
{
    char type = (char) DataType::INT32;
    write(&type, sizeof(char));
    write((char *) &value, sizeof(int32_t));
}

void Datastream::write(int64_t value)
{
    char type = (char) DataType::INT64;
    write(&type, sizeof(char));
    write((char *) &value, sizeof(int64_t));
}

void Datastream::write(float value)
{
    char type = (char) DataType::FLOAT;
    write(&type, sizeof(char));
    write((char *) &value, sizeof(float));
}

void Datastream::write(double value)
{
    char type = (char) DataType::DOUBLE;
    write(&type, sizeof(char));
    write((char *) &value, sizeof(double));
}

void Datastream::write(const char * value)
{
    char type = (char) DataType::STRING;
    write(&type, sizeof(char));
    int32_t len = std::strlen(value);
    write(len);
    write(value, len);
}

void Datastream::write(const string & value)
{
    char type = (char) DataType::STRING;
    write(&type, sizeof(char));
    int len = value.size();
    write(len);
    write(value.data(), len);
}

Datastream & Datastream::operator<<(bool value)
{
    write(value);
    return *this;
}

Datastream & Datastream::operator<<(char value)
{
    write(value);
    return *this;
}

Datastream & Datastream::operator<<(int32_t value)
{
    write(value);
    return *this;
}

Datastream & Datastream::operator<<(int64_t value)
{
    write(value);
    return *this;
}

Datastream & Datastream::operator<<(float value)
{
    write(value);
    return *this;
}

Datastream & Datastream::operator<<(double value)
{
    write(value);
    return *this;
}

Datastream & Datastream::operator<<(const char * value)
{
    write(value);
    return *this;
}

Datastream & Datastream::operator<<(const string & value)
{
    write(value);
    return *this;
}

void Datastream::show() const
{
    int size = m_buff.size();
    int i = 0;
    while (i < size)
    {
        switch ((DataType) m_buff[i++])
        {
            case DataType::BOOL:
            {
                std::cout << std::boolalpha << *((bool *) &m_buff[i]) << " ";
                i += sizeof(char);
            }
                break;
            case DataType::CHAR:
            {
                std::cout << m_buff[i] << " ";
                i += sizeof(char);
            }
                break;
            case DataType::INT32:
            {
                std::cout << *((int32_t *) &m_buff[i]) << " ";
                i += sizeof(int32_t);
            }
                break;
            case DataType::INT64:
            {
                std::cout << *((int64_t *) &m_buff[i]) << " ";
                i += sizeof(int64_t);
            }
                break;
            case DataType::FLOAT:
            {
                std::cout << *((float *) &m_buff[i]) << " ";
                i += sizeof(float);
            }
                break;
            case DataType::DOUBLE:
            {
                std::cout << *((double *) &m_buff[i]) << " ";
                i += sizeof(double);
            }
                break;
            case DataType::STRING:
            {
                if (DataType::INT32 == (DataType) m_buff[i])
                {
                    i += sizeof(char);
                    int len = *((int32_t *) &m_buff[i]);
                    i += sizeof(int32_t);
                    std::cout << string(&m_buff[i], len) << " ";
                    i += len;
                } else
                {
                    throw std::logic_error("parse string error");
                }
            }
                break;
            default:
                break;
        }

    }
    std::cout << std::endl;
}

bool Datastream::read(bool & value)
{
    if (DataType::BOOL != (DataType) m_buff[m_pos])
    {
        return false;
    }
    m_pos += sizeof(char);
    value = *((bool *) &m_buff[m_pos]);
    m_pos += sizeof(char);
    return true;
}

bool Datastream::read(char & value)
{
    if (DataType::CHAR != (DataType) m_buff[m_pos])
    {
        return false;
    }
    m_pos += sizeof(char);
    value = m_buff[m_pos];
    m_pos += sizeof(char);
    return true;
}

bool Datastream::read(int32_t & value)
{
    if (DataType::INT32 != (DataType) m_buff[m_pos])
    {
        return false;
    }
    m_pos += sizeof(char);
    value = *((int32_t *) &m_buff[m_pos]);
    m_pos += sizeof(int32_t);
    return true;
}

bool Datastream::read(int64_t & value)
{
    if (DataType::INT64 != (DataType) m_buff[m_pos])
    {
        return false;
    }
    m_pos += sizeof(char);
    value = *((int64_t *) &m_buff[m_pos]);
    m_pos += sizeof(int64_t);
    return true;
}

bool Datastream::read(float & value)
{
    if (DataType::FLOAT != (DataType) m_buff[m_pos])
    {
        return false;
    }
    m_pos += sizeof(char);
    value = *((float *) &m_buff[m_pos]);
    m_pos += sizeof(float);
    return true;
}

bool Datastream::read(double & value)
{
    if (DataType::DOUBLE != (DataType) m_buff[m_pos])
    {
        return false;
    }
    m_pos += sizeof(char);
    value = *((double *) &m_buff[m_pos]);
    m_pos += sizeof(double);
    return true;
}

bool Datastream::read(string & value)
{
    if (DataType::STRING != (DataType) m_buff[m_pos])
    {
        return false;
    }
    m_pos += sizeof(char);
    int len = 0;
    read(len);
    value.assign(&m_buff[m_pos], len);
    return true;
}

Datastream & Datastream::operator>>(bool & value)
{
    read(value);
    return *this;
}

Datastream & Datastream::operator>>(char & value)
{
    read(value);
    return *this;
}

Datastream & Datastream::operator>>(int32_t & value)
{
    read(value);
    return *this;
}

Datastream & Datastream::operator>>(int64_t & value)
{
    read(value);
    return *this;
}

Datastream & Datastream::operator>>(float & value)
{
    read(value);
    return *this;
}

Datastream & Datastream::operator>>(double & value)
{
    read(value);
    return *this;
}

Datastream & Datastream::operator>>(string & value)
{
    read(value);
    return *this;
}
