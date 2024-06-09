#include <iostream>

#include <utilities/ini_file.h>

using namespace demo::utilities;

int main()
{
//    std::cout << "ini parser demo" << std::endl;

    IniFile ini("./../main.ini");

    ini.show();

//    const string & ip = ini.get("server", "ip");
//    std::cout << ip << std::endl;
//
//    int port = ini.get("server", "port");
//    std::cout << port << std::endl;

//    const string & ip = ini["server"]["ip"];
//    const string & port = ini["server"]["port"];
//    std::cout << ip << ":" << port << std::endl;

//    ini.set("server", "ip", "192.168.1.1");
//    ini.set("server", "port", 8080);
//    ini.show();

//    ini["server"]["ip"] = "192.168.1.1";
//    ini["server"]["port"] = 8080;
//    ini.show();

//    ini.save("./../main-bak.ini");

//    if (ini.has("server"))
//    {
//        std::cout << "ini[server] is exist" << std::endl;
//    }

//    if (ini.has("server", "ip"))
//    {
//        std::cout << "ini[server][ip] is exist" << std::endl;
//    }

//    ini.remove("server", "ip");
//    ini.show();

//    ini.remove("server");
//    ini.show();

//    ini.clear();
//    ini.show();

    return 0;
}
