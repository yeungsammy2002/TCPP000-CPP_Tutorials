#pragma once

#include <string>
#include <sstream>

using std::string;

namespace demo {
namespace utilities {

class Value
{
public:
    Value() = default;

    ~Value() = default;

    Value(bool value);

    Value(int value);

    Value(double value);

    Value(const char * value);

    Value(const string & value);

    Value(const Value & other);

    Value & operator=(const Value & other);

    bool operator==(const Value & other) const;

    bool operator!=(const Value & other) const;

    operator bool();

    operator int();

    operator double();

    operator string();

private:
    string m_value;
};

}
}