#include <iostream>

#include <json/json.h>

using namespace demo::json;

int main()
{
//    std::cout << "json parser demo" << std::endl;

    Json v1;
    Json v2 = true;
    Json v3 = 123;
    Json v4 = 1.23;
    Json v5 = "hello world";

    bool r1 = v2;
    int r2 = v3;
    double r3 = v4;
    string r4 = v5;

    Json arr;
    arr.append(r1);
    arr.append(r2);
    arr[2] = r3;
    arr[3] = r4;
    arr.show();

    Json obj;
    obj["bool"] = r1;
    obj["int"] = r2;
    obj["double"] = r3;
    obj["string"] = r4;
    obj.show();

    std::cout << std::boolalpha;
    std::cout << arr.has(0) << std::endl;
    std::cout << arr.has(4) << std::endl;

    std::cout << obj.has("string") << std::endl;
    std::cout << obj.has("string123") << std::endl;

    arr.remove(0);
    arr.show();

    obj.remove("string");
    obj.show();

    return 0;
}