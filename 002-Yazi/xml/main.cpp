#include <iostream>

#include <xml/xml.h>

using namespace demo::xml;

int main()
{
//    std::cout << "xml parser demo" << std::endl;

    Xml root("root");
    std::cout << root.name() << std::endl;

    root.name("students");
    std::cout << root.name() << std::endl;

    root.show();

    Xml s1("student");
    s1.text("a good student");
    s1.attr("id", 1);
    s1.show();
    s1.attr("class", 101);
    s1.show();

    std::cout << s1.text() << std::endl;
    std::cout << string(s1.attr("id")) << std::endl;
    std::cout << string(s1.attr("class")) << std::endl;

    root.append(s1);
    root.show();

    Xml s2("student");
    s2.text("a bad student");
    s2.attr("id", 2);
    s2.attr("class", 102);
    root[1] = s2;
    root.show();

    root[0].show();
    root[1].show();

    root["student"].show();

//    root.remove(0);
//    root["student"].show();

    root.remove("student");
    root.show();

    return 0;
}