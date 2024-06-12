#include <iostream>

#include <json/json.h>

using namespace demo::json;

int main()
{
//    std::cout << "Json parser demo" << std::endl;

    Json i1;
    i1.show();

    Json i2(Json::JSON_INT);
    i2 = 24;
//    i2 = 24.5;
    i2.show();

    Json i3 = true;
    i3.show();

    Json i4 = false;
    i4.show();

    Json i5 = 123;
    i5.show();

    Json i6 = 1.23;
    i6.show();

    Json i7 = "hello world";
    i7.show();

    string str = "kitty";
    Json i8 = str;
    i8.show();

    bool v1 = i3;
    int v2 = i5;
    double v3 = i6;
    string v4 = i7;
    std::cout << v1 << "," << v2 << "," << v3 << "," << v4 << std::endl;

//    Json arr(Json::JSON_ARRAY);
//    arr.append(i1);
//    arr.append(i2);
//    arr.append(i3);
//    arr.append(i4);
//    arr.append(i5);
//    arr.append(i6);
//    arr.append(i7);
//    arr.append(i8);
//    arr.show();

    Json arr2(Json::JSON_ARRAY);
    arr2.append(true);
    arr2.append(false);
    arr2.append(123);
    arr2.append(1.23);
    arr2.append("hello world");
    arr2.append(str);
//    arr2.show();

    std::cout << arr2[4] << std::endl;

//    Json obj(Json::JSON_OBJECT);
//    obj["bool"] = false;
//    obj["int"] = 123;
//    obj["double"] = 1.23;
//    obj["string"] = "hello world";
//    obj["str"] = str;
//    obj.show();

//    std::cout << obj["bool"] << std::endl;


    return 0;
}
