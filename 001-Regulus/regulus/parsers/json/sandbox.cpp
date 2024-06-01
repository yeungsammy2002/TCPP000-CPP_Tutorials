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

//    Json arr(Json::JSON_ARRAY);
//    arr.append(true);
//    arr.append(123);
//    arr.append(1.23);
//    arr.append("hello world");
//    arr.show();


//    Json arr1(Json::JSON_ARRAY);
//
//    Json str_arr(Json::JSON_ARRAY);
//    str_arr.append("sam");
//    str_arr.append("jack");
//    str_arr.append("chris");
//    str_arr.append("john");
//    str_arr1.show();
//
//    arr1.append(str_arr);
//
//    Json arr2(arr1);
//    arr1.show();
//    arr2.show();



// check if the deep copy is work on JSON_ARRAY

//    arr1[0].show();
//    arr2[0].show();

//    arr1[0][0].show();
//    arr2[0][0].show();


//    arr1[0][0] = "jason";
//    arr1.show();    // [["jason","jack","chris","john"]]
//    arr2.show();    // [["sam","jack","chris","john"]]


// check if the deep copy is work properly on JSON_OBJECT

//    Json obj1(Json::JSON_OBJECT);

//    obj1["arr"] = arr1;
//    obj1.show();

//    Json obj2(obj1);
//    obj2.show();

//    obj1["arr"][0][0] = "jason";
//    obj1["arr"][0][1].show();
//    obj1.show();
//    obj2.show();


    return 0;
}