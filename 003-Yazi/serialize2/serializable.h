#pragma once

namespace demo {
namespace serialize {

class DataStream;

class Serializable
{
public:
    virtual void serialize(DataStream & stream) const = 0;
    virtual bool deserialize(DataStream & stream) = 0;
};





#define SERIALIZE(...)                              \
    void serialize(DataStream & stream) const       \
    {                                               \
        char type = DataStream::CUSTOM;             \
        stream.write((char *)&type, sizeof(char));  \
        stream.write_args(__VA_ARGS__);             \
    }                                               \
                                                    \
    bool deserialize(DataStream & stream)           \
    {                                               \
        char type;                                  \
        stream.read(&type, sizeof(char));           \
        if(type != DataStream::CUSTOM)              \
        {                                           \
            return false;                           \
        }                                           \
        stream.read_args(__VA_ARGS__);              \
        return true;                                \
    }





}
}