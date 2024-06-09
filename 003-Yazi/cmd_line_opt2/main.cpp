#include <iostream>

#include <utilities/option.h>

using namespace demo::utilities;

int main(int argc, char * argv[])
{
//    std::cout << "command line option 2" << std::endl;

    Option opt;

    opt.add("a", Option::OPT_NO);
    opt.add("b", Option::OPT_REQUIRED);
    opt.add("c", Option::OPT_OPTIONAL);

//    opt.add("name", Option::OPT_REQUIRED);
//    opt.add("age", Option::OPT_OPTIONAL);
//    opt.add("gender", Option::OPT_OPTIONAL);

    opt.parse(argc, argv);
    opt.show();

//    bool a = opt.get_bool("a");
//    int b = opt.get_int("b");
//    string c = opt.get_string("c");
//    std::cout << "a=" << a << ",b=" << b << ",c=" << c << std::endl;

    bool a = opt.get("a");
    int b = opt.get("b");
    string c = opt.get("c");
    std::cout << "a=" << a << ",b=" << b << ",c=" << c << std::endl;

    return 0;
}
