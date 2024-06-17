#include <iostream>

#include <xml/xml.h>

using namespace demo::xml;

int main()
{
//    Xml x;

//    x.name("students");
//    x.text("hello world");
//    std::cout << x << std::endl;

//    x.name("students");
//    Xml c1("student");
//    c1.text("c1");
//    Xml c2("student");
//    c2.text("c2");
//    Xml c3("student");
//    c3.text("c3");
//
//    x.append(c1);
//    x.append(c2);
//    x.append(c3);
//    std::cout << x << std::endl;
//
//    x.remove(0);
//    std::cout << x << std::endl;

//    x.name("students");
//    Xml c1("student");
//    c1.text("c1");
//    Xml c2("student");
//    c2.text("c2");
//    Xml c3("student");
//    c3.text("c3");
//
//    x.append(c1);
//    x.append(c2);
//    x.append(c3);
//
//    for (auto it = x.begin(); it != x.end(); ++it)
//    {
//        std::cout << *it << std::endl;
//    }
//
//    Xml e = x;
//    e.name("e");
//    std::cout << x << std::endl;
//    std::cout << e << std::endl;
//
//    x.save("./../test.xml");

//    Parser p;
//    p.load("./../test.xml");
//    Xml x = p.parse();
//
//    std::cout << x << std::endl;

    Xml x;
//    x.load("./../test.xml");
    x.load("./../t.xml");
    std::cout << x << std::endl;


    return 0;
}
