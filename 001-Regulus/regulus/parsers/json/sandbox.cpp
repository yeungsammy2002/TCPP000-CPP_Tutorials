#include <iostream>

#include "json.h"

using namespace regulus::parsers;

int main()
{
//    std::cout << "json parser sandbox" << std::endl;

//    Json v1 = true;
//    Json v2 = 123;
//    Json v3 = 1.23;
//    Json v4 = "hello world";
//    string s = "kitty";
//    Json v5 = s;


//    bool b = v1;
//    std::cout << std::boolalpha << b << std::endl;
//    b = false;
//    std::cout << std::boolalpha << b << std::endl;

//    b = v2;   // test assertion fail

    Json arr(Json::JSON_ARRAY);
    arr.append(true);
//    arr.append(123);
//    arr.append(1.23);
//    arr.append("hello world");
    arr.show();

    return 0;
}