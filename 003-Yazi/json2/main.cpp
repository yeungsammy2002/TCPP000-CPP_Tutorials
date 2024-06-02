#include <iostream>

#include <json/json.h>
//#include <json/parser.h>

using namespace demo::json;

int main()
{
//    Json v1;
//    Json v2 = true;
//    Json v3 = 123;
//    Json v4 = 1.23;
//    Json v5 = "hello world";

//    v1.show();
//    v2.show();
//    v3.show();
//    v4.show();
//    v5.show();

//    std::cout << v1 << std::endl;
//    std::cout << v2 << std::endl;
//    std::cout << v3 << std::endl;
//    std::cout << v4 << std::endl;
//    std::cout << v5 << std::endl;

//    bool b = v2.as_bool();
//    int i = v3.as_int();
//    double d = v4.as_double();
//    string s = v5.as_string();

//    bool b = v2;
//    int i = v3;
//    double d = v4;
//    string s = v5;

//    Json v = "hello world";
//    v = 123;

//    Json v;
//    v.append(true);
//    v.append(123);
//    v.append("hello world");
//    v.append(Json());
//    v.show();

//    bool b = v.get(0);
//    int i = v[1];
//    string s = v[2];
//
//    std::cout << b << std::endl;
//    std::cout << i << std::endl;
//    std::cout << s << std::endl;

//    std::cout << v[0] << std::endl;
//    v.remove(0);
//    v.show();
//    std::cout << v << std::endl;

//    for (auto it = v.begin(); it != v.end(); ++it)
//    {
//        std::cout << (*it) << std::endl;
//    }


//    Json v;
//    v["name"] = "jack";
//    v["age"] = 18;
//    v["gender"] = "male";

//    std::cout << v << std::endl;

//    v.remove("gender");
//
//    std::cout << v << std::endl;
//
//    v["work"] = "c++";
//    std::cout << v << std::endl;

//    string name = v["name"];
//    string name = v.get("name");

//    std::cout << name << std::endl;
//
//    int age = v["age"];
//    std::cout << age << std::endl;


//    Json v1 = "hello world";
//    Json v2 = v1;
//
//    v1.clear();
//
//    std::cout << v1 << std::endl;
//    std::cout << v2 << std::endl;

//    string str = "null";
//    string str = "true";
//    string str = "false";
//    string str = "123";
//    string str = "12.3";
//    string str = "0.123";

//    string str = "\"hello world\"";
//    string str = "\"hello \n world\"";
//    string str = "\"hello \t world\"";
//    string str = "\"hello \\ world\"";

//    string str = "[1, 2, 3]";
//    string str = "[1,2,3,]";
//    string str = "[1, 2, 3, null, true, \"hello world\"]";

//    string str = "{\"name\": \"jack\", \"age\": 18}";

//    Parser p;
//    p.load(str.c_str(), str.size());
//    Json v = p.parse();
//    std::cout << v << std::endl;

//    Json v;
//    v.parse(str.c_str(), str.size());
//    v.parse("./../test.json");
//    std::cout << v << std::endl;

//    Json v1 = "hello world";
//    Json v2 = v1;
//    v1.clear();

//    Json v2 = std::move(v1);
//
//    std::cout << v1 << std::endl;
//    std::cout << v2 << std::endl;

    Json v;
    v.parse("./../test.json");

    return 0;
}
