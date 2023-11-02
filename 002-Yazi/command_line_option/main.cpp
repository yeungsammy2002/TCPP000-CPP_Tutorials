#include <iostream>

#include <utilities/option.h>

using namespace demo::utilities;

int main(int argc, char * argv[])
{
//    std::cout << "command line option demo" << std::endl;

    Option opt;

//    opt.add("name", Option::OPT_REQUIRED);
//    opt.add("age", Option::OPT_OPTIONAL);
//    opt.add("gender", Option::OPT_OPTIONAL);
//    opt.parse(argc, argv);
//    opt.show();

    opt.add("a", Option::OPT_NONE);
    opt.add("b", Option::OPT_REQUIRED);
    opt.add("c", Option::OPT_OPTIONAL);

    opt.parse(argc, argv);

    opt.show();
    std::cout << std::endl;

    std::cout << std::boolalpha;
    std::cout << "a:" << opt.has("a") << std::endl;
    std::cout << "x:" << opt.has("x") << std::endl;

    bool a = opt.get_bool("a");
    int b = opt.get_int("b");
    string c = opt.get_string("c");

    std::cout << "a=" << a << ", b=" << b << ", c=" << c << std::endl;

    return 0;
}