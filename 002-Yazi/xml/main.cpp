#include <iostream>

#include <xml/Xml.h>

using namespace demo::xml;

int main() {
//    std::cout << "xml demo" << std::endl;

    Xml root("students");
//    std::cout << root.name() << std::endl;

    Xml s1;
    s1.name("student");
    s1.attr("id", "1");
    s1.attr("class", "101");
    s1.text("a good student");
//    std::cout << s1.text() << std::endl;
//    std::cout << s1.str() << std::endl;

//    root.append(s1);
//    std::cout << root.str() << std::endl;
//    std::cout << root[0].str() << std::endl;

//    Xml s2;
//    s2.name("student");
//    s2.attr("id", "2");
//    s2.attr("class", "102");
//    s2.text("a bad student");
//
//    root[0] = s1;
//    root[1] = s2;
//    std::cout << root.str() << std::endl;
//    root.remove(0);
//    root.remove(1);
//    root.remove("student");
//    std::cout << root.str() << std::endl;

//    for (auto it = root.begin(); it != root.end(); ++it) {
//        std::cout << it->str() << std::endl;
//    }
//
//    int size = root.size();
//    std::cout << size << std::endl;
//    for (int i = 0; i < size; ++i) {
//        std::cout << root[i].str() << std::endl;
//    }
}