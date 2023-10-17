#include <serialize/DataStream.h>

using namespace demo::serialize;

void DataStream::reserve(int len)
{
    int size = m_buf.size();
    int cap = m_buf.capacity();
    if (size + len > cap)
    {
        while (size + len > cap)
        {
            if (0 == cap)
                cap = 1;
            else
                cap *= 2;
        }
        m_buf.reserve(cap);
    }
}

void DataStream::write(const char * data, int len)
{
    reserve(len);
    int size = m_buf.size();
    m_buf.resize(size + len);
    std::memcpy(&m_buf[size], data, len);
}

void DataStream::write(bool value)
{
    char type = DataType::BOOL;
    write((char *) &type, sizeof(char));
    write((char *) &value, sizeof(bool));
}

void DataStream::write(char value)
{
    char type = DataType::CHAR;
    write((char *) &type, sizeof(char));
    write((char *) &value, sizeof(char));
}

void DataStream::write(std::int32_t value)
{
    char type = DataType::INT32;
    write((char *) &type, sizeof(char));
    write((char *) &value, sizeof(std::int32_t));
}

void DataStream::write(std::int64_t value)
{
    char type = DataType::INT64;
    write((char *) &type, sizeof(char));
    write((char *) &value, sizeof(std::int64_t));
}

void DataStream::write(float value)
{
    char type = DataType::FLOAT;
    write((char *) &type, sizeof(char));
    write((char *) &value, sizeof(float));
}

void DataStream::write(double value)
{
    char type = DataType::DOUBLE;
    write((char *) &type, sizeof(char));
    write((char *) &value, sizeof(double));
}

void DataStream::write(const char * value)
{
    char type = DataType::STRING;
    write((char *) &type, sizeof(char));
    int len = std::strlen(value);
    write(len);
    write(value, len);
}

void DataStream::write(const string & value)
{
    char type = DataType::STRING;
    write((char *) &type, sizeof(char));
    int len = value.size();
    write(len);
    write(value.data(), len);
}

void DataStream::show() const
{
    int size = m_buf.size();
    std::cout << "data size = " << size << std::endl;
    int i = 0;
    while (i < size)
    {
        switch ((DataType) m_buf[i])
        {
            case DataType::BOOL:
                i += sizeof(char);
                if (*((bool *) (&m_buf[i])))
                    std::cout << "true" << std::endl;
                else
                    std::cout << "false" << std::endl;
                i += sizeof(bool);
                break;
            case DataType::CHAR:
                i += sizeof(char);
                std::cout << m_buf[i] << std::endl;
                i += sizeof(char);
                break;
            case DataType::INT32:
                i += sizeof(char);
                std::cout << *((std::int32_t *) (&m_buf[i])) << std::endl;
                i += sizeof(std::int32_t);
                break;
            case DataType::INT64:
                i += sizeof(char);
                std::cout << *((std::int64_t *) (&m_buf[i])) << std::endl;
                i += sizeof(std::int64_t);
                break;
            case DataType::FLOAT:
                i += sizeof(char);
                std::cout << *((float *) (&m_buf[i])) << std::endl;
                i += sizeof(float);
                break;
            case DataType::DOUBLE:
                i += sizeof(char);
                std::cout << *((double *) (&m_buf[i])) << std::endl;
                i += sizeof(double);
                break;
            case DataType::STRING:
            {
                i += sizeof(char);
                if (DataType::INT32 == (DataType) m_buf[i])
                {
                    i += sizeof(char);
                    int len = *((std::int32_t *) (&m_buf[i]));
                    i += sizeof(std::int32_t);
                    std::cout << string(&m_buf[i], len) << std::endl;
                    i += len;
                } else
                    throw std::logic_error("parse string error");
                break;
            }
            default:
                break;
        }
    }
}

DataStream & DataStream::operator<<(bool value)
{
    write(value);
    return *this;
}

DataStream & DataStream::operator<<(char value)
{
    write(value);
    return *this;
}

DataStream & DataStream::operator<<(std::int32_t value)
{
    write(value);
    return *this;
}

DataStream & DataStream::operator<<(std::int64_t value)
{
    write(value);
    return *this;
}

DataStream & DataStream::operator<<(float value)
{
    write(value);
    return *this;
}

DataStream & DataStream::operator<<(double value)
{
    write(value);
    return *this;
}

DataStream & DataStream::operator<<(const char * value)
{
    write(value);
    return *this;
}

DataStream & DataStream::operator<<(const string & value)
{
    write(value);
    return *this;
}

bool DataStream::read(bool & value)
{
    if (DataType::BOOL != (DataType) m_buf[m_pos])
        return false;
    m_pos += sizeof(char);
    value = *((bool *) (&m_buf[m_pos]));
    m_pos += sizeof(bool);
    return true;
}

bool DataStream::read(char & value)
{
    if (DataType::CHAR != (DataType) m_buf[m_pos])
        return false;
    m_pos += sizeof(char);
    value = m_buf[m_pos];
    m_pos += sizeof(char);
    return true;
}

bool DataStream::read(std::int32_t & value)
{
    if (DataType::INT32 != (DataType) m_buf[m_pos])
        return false;
    m_pos += sizeof(char);
    value = *((std::int32_t *) (&m_buf[m_pos]));
    m_pos += sizeof(std::int32_t);
    return true;
}

bool DataStream::read(std::int64_t & value)
{
    if (DataType::INT64 != (DataType) m_buf[m_pos])
        return false;
    m_pos += sizeof(char);
    value = *((std::int64_t *) (&m_buf[m_pos]));
    m_pos += sizeof(std::int64_t);
    return true;
}

bool DataStream::read(float & value)
{
    if (DataType::FLOAT != (DataType) m_buf[m_pos])
        return false;
    m_pos += sizeof(char);
    value = *((float *) (&m_buf[m_pos]));
    m_pos += sizeof(float);
    return true;
}

bool DataStream::read(double & value)
{
    if (DataType::DOUBLE != (DataType) m_buf[m_pos])
        return false;
    m_pos += sizeof(char);
    value = *((double *) (&m_buf[m_pos]));
    m_pos += sizeof(double);
    return true;
}

bool DataStream::read(string & value)
{
    if (DataType::STRING != (DataType) m_buf[m_pos])
        return false;
    m_pos += sizeof(char);
    int len;
    if (!read(len))
        return false;
    if (len < 0)
        return false;
    value.assign((char *) &m_buf[m_pos], len);
    m_pos += len;
    return true;
}

DataStream & DataStream::operator>>(bool & value)
{
    read(value);
    return *this;
}

DataStream & DataStream::operator>>(char & value)
{
    read(value);
    return *this;
}

DataStream & DataStream::operator>>(std::int32_t & value)
{
    read(value);
    return *this;
}

DataStream & DataStream::operator>>(std::int64_t & value)
{
    read(value);
    return *this;
}

DataStream & DataStream::operator>>(float & value)
{
    read(value);
    return *this;
}

DataStream & DataStream::operator>>(double & value)
{
    read(value);
    return *this;
}

DataStream & DataStream::operator>>(string & value)
{
    read(value);
    return *this;
}
