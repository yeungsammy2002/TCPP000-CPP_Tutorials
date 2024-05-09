#include <iostream>

#include <ini/ini.h>

using namespace demo::ini;

int main()
{
//    std::cout << "ini parser demo" << std::endl;

    Ini ini;
    ini.load("./main.ini");

    ini.show();

    std::cout << string(ini.get("server", "ip")) << ":" << string(ini.get("server", "port")) << std::endl;

    std::cout << string(ini["server"]["ip"]) << ":" << string(ini["server"]["port"]) << std::endl;

    ini.set("server", "timeout", 1000);
    ini.show();

    std::cout << std::boolalpha;

    std::cout << ini.has("server", "ip") << std::endl;
    std::cout << ini.has("server", "flag") << std::endl;
    std::cout << ini.has("server") << std::endl;
    std::cout << ini.has("server123") << std::endl;

//    ini.remove("server", "ip");
//    ini.show();

//    ini.remove("server");
//    ini.show();

//    ini.clear();

    ini.save("./temp.ini");

    return 0;
}