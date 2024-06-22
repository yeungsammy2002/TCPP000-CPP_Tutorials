#include <iostream>

#include <utilities/option.h>

using namespace demo::utilities;

int main(const int argc, const char * argv[])
{
//    std::cout << "command line option" << std::endl;

    Option opt;





//    opt.add("name", Option::OPT_NO);
//    Option::Type t = opt.type("name");
////    Option::Type t2 = opt.type("age");




//    opt.add("a", Option::OPT_NO);
//    opt.add("b", Option::OPT_OPTIONAL);
//    opt.add("c", Option::OPT_REQUIRED);





    opt.add("name", Option::OPT_REQUIRED);
    opt.add("age", Option::OPT_OPTIONAL);
    opt.add("local", Option::OPT_NO);





    opt.parse(argc, argv);
//    opt.show();


    std::cout << std::boolalpha << opt.has("name") << std::endl;
    std::cout << std::boolalpha << opt.has("age") << std::endl;

    std::cout << opt.get("name") << std::endl;



    return 0;
}
