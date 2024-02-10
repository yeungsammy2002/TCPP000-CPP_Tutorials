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

    bool b = v2;
    int i = v3;
    double d = v4;
    string s = v5;

    Json arr;
    arr[0] = true;
    arr[1] = 123;
    arr.append(1.23);
    arr.append("hello world");
    std::cout << arr.str() << std::endl;

    Json obj;
    obj["bool"] = true;
    obj["int"] = 123;
    obj["double"] = 1.23;
    obj["string"] = "hello world";
    std::cout << obj.str() << std::endl;

    for (auto it = arr.begin(); it != arr.end(); ++it)
    {
        std::cout << it->str() << " ";
    }

    std::cout << std::endl << std::boolalpha;
    std::cout << arr.has(0) << std::endl;
    std::cout << arr.has(4) << std::endl;
    arr.remove(0);
    std::cout << arr.str() << std::endl;

    std::cout << obj.has("string") << std::endl;
    std::cout << obj.has("string123") << std::endl;
    obj.remove("string");
    std::cout << obj.str() << std::endl;

    arr.append(obj);
    std::cout << arr.str() << std::endl;
    arr.remove(arr.size() - 1);
    std::cout << arr.str() << std::endl;
    arr.clear();
    std::cout << arr.str() << std::endl;

    return 0;
}