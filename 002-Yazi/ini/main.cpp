#include <iostream>

#include <IniFile.h>

int main()
{
//    std::cout << "ini demo" << std::endl;

    Value v1(false);
    Value v2(123);
    Value v3(1.23);
    Value v4("hello world");
    Value v5(string("keep in mind"));

    Value v6;
    v6 = true;
    Value v7;
    v7 = 456;
    Value v8;
    v8 = 4.56;
    Value v9;
    v9 = "greetings";
    Value v10;
    v10 = string("overthink");

    bool b = v1;
    int i = v2;
    double d = v3;
    string s = v4;

    IniFile ini;
    ini.load("./main.ini");

    const string & ip = ini.get("server", "ip");
    int port = ini.get("server", "port");

    bool hasServer = ini.has("server");
    bool hasX = ini.has("x");

    bool hasIp = ini.has("server", "ip");
    bool hasXX = ini.has("x", "x");

    ini.set("server", "flag", true);
    bool hasFlag = ini.has("server", "flag");
    bool flag = ini.get("server", "flag");

    ini.set("x");
    ini.set("x", "x", "x");
    ini["server"]["timeout"] = 1000;
    ini.show();
    ini.remove("x", "x");
    ini.show();
    ini.remove("x");
    ini.show();

    ini.save("./temp.ini");

    ini.clear();
//    ini.save("./temp.ini");

    return 0;
}