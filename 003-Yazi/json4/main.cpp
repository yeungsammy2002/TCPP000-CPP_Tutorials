#include <iostream>

#include <json/json.h>
#include <json/parser.h>

int main()
{
    std::cout << std::boolalpha;

    Json v1;
    Json v2 = true;
    Json v3 = 123;
    Json v4 = 1.23;
    Json v5 = "hello world";

//    std::cout << v1 << " " << v2 << " " << v3 << " " << v4 << " " << v5 << std::endl;

    bool r2 = v2;
    int r3 = v3;
    double r4 = v4;
    string r5 = v5;

//    v1 = false;
//    std::cout << v1 << " ";
//    v1 = 456;
//    std::cout << v1 << " ";
//    v1 = 4.56;
//    std::cout << v1 << " ";
//    v1 = "kitty";
//    std::cout << v1 << std::endl;

//    Json v5a;
//    v5a = v5;
//    std::cout << v5a << std::endl;

//    Json v5b(v5);
//    std::cout << v5b << std::endl;

    Json v6;
    v6.append(Json());
    v6.append(true);
    v6.append(false);
    v6.append(1);
    v6.append(2);
    v6.append(3);
    v6.append(1.23);
    v6.append("hello world");
    v6[8] = "kitty";
//    std::cout << v6 << std::endl;

//    std::cout << v6[0] << " "
//              << v6[1] << " "
//              << v6[2] << " "
//              << v6[3] << " "
//              << v6[4] << " "
//              << v6[5] << " "
//              << v6[6] << " "
//              << v6[7] << std::endl;

//    std::cout << v6.has(0) << " " << v6.has(8) << std::endl;

//    std::cout << v6.get(0) << " " << v6.get(7) << std::endl;

//    std::cout << v6.size() << std::endl;

//    v6.remove(7);
//    std::cout << v6 << std::endl;

    Json v7;
    v7["name"] = "Jack";
    v7["age"] = 18;
    v7["sex"] = "male";
//    std::cout << v7 << std::endl;
    v7.remove("sex");
//    std::cout << v7 << std::endl;
    v7["other"] = v6;
//    std::cout << v7 << std::endl;
//    std::cout << v6 << std::endl;

    v6.append(v7);
//    std::cout << v6 << std::endl;

//    Parser p;
//    string s1 = "null";
//    p.load(s1.c_str(), s1.size());
//    Json v8 = p.parse();
//    std::cout << v8 << std::endl;

////    string s2 = "true";
//    string s2 = "false";
//    p.load(s2.c_str(), s2.size());
//    Json v9 = p.parse();
//    std::cout << v9 << std::endl;

////    string s3 = "123";
////    string s3 = "-123";
////    string s3 = "0.123";
//    string s3 = "12.3";
//    p.load(s3.c_str(), s3.size());
//    Json v10 = p.parse();
//    std::cout << v10 << std::endl;

//    string s4 = "\"hello world\"";
//    p.load(s4.c_str(), s4.size());
//    Json v11 = p.parse();
//    std::cout << v11 << std::endl;
//
//    string s5 = "[ null, true, 123, 1.23, \"hello world\" ]";
//    p.load(s5.c_str(), s5.size());
//    Json v12 = p.parse();
//    std::cout << v12 << std::endl;
//
//    string s6 = "{ \"name\": \"Jack\", \"age\": 18 }";
//    p.load(s6.c_str(), s6.size());
//    Json v13 = p.parse();
//    std::cout << v13 << std::endl;

    string s6 = "[ null, true, 123, 1.23, \"hello world\", [ 1, 2, 3 ], { \"name\" : \"Jack\", \"age\" : 18 } ]";
    Json v14;
    v14.parse(s6.c_str(), s6.size());
//    std::cout << v14 << std::endl;
//    std::cout << v14[6] << std::endl;

    string s7 = "{ \"name\": \"Jack\", \"age\": 18, \"other\": [null, true, 123, 1.23, \"hello world\" ] }";
    Json v15;
    v15.parse(s7.c_str(), s7.size());
//    std::cout << v15 << std::endl;
//    std::cout << v15["other"] << std::endl;

    Json v16;
    v16.parse("./../test.json");
    std::cout << v16 << std::endl;

    return 0;
}

