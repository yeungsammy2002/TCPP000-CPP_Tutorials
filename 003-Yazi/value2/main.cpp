#include <iostream>

#include <utilities/value.h>

using namespace demo::utilities;

int main()
{
    Value v1 = true;
    Value v2 = false;
    Value v3 = 123;
    Value v4 = 1.23f;
    Value v5 = 1.23;
    Value v6 = "hello world";

    Value v7;
    v7 = "kitty";

    bool x1 = v1;
//    int x2 = v1;

    std::cout << v3 << std::endl;
    std::cout << v2 << std::endl;
    std::cout << v4 << std::endl;

    v6.show();

    return 0;
}
