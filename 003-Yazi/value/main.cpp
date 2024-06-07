#include <iostream>

#include <utilities/value.h>
//#include <sstream>
//#include <string>

using namespace demo::utilities;

class Test
{
public:
//    int foo(Value & value)
//    {
//        int i = value;
//        return i;
//    }

    int foo(const Value & value) const
    {
        int i = value;
        return i;
    }
};

int main()
{
//    std::cout << "Value demo" << std::endl;

//    Value v = 123;
//    Value v = true;
//    Value v = 1.23;
//    Value v = 1.23f;
//    Value v = "hello world";
//    v.show();

//    Value v = "hello world";
//    string s = v;
//    std::cout << s << std::endl;

//    Value v = 123;
//    int i = v;
//    std::cout << i << std::endl;

//    Value v = 1.23;
//    float f = v;
//    std::cout << f << std::endl;

//    Value v = true;
//    bool b = v;
//    std::cout << std::boolalpha << b << std::endl;

    Test t;
    Value v = 123;
    t.foo(v);

    return 0;
}
