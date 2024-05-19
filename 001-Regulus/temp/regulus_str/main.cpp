#include <iostream>

#include <utilities/str.h>

using namespace regulus::utilities;

int main()
{
    std::cout << std::boolalpha;


// check are_alpha() work properly
//    string s = "hello";
//    string s = "hello world";
//    bool are_alpha = Str::are_alpha(s);
//    std::cout << are_alpha;


// check are_digit() work properly
//    string s = "1213456789";
//    string s = "12.23";
//    string s = "12A";
//    bool are_digit = Str::are_digit(s);
//    std::cout << are_digit << std::endl;

// check trim() work properly
//    string s = "   127.0.0.1  ";
//    string s = "127.0.0.1";
//    s = Str::trim(s);
//    std::cout << "[" << s << "]" << std::endl;

// check trim() specific characters work properly
//    string s = "[ 127.0.0.1 ]";
//    string s = "127.0.0.1";
//    s = Str::trim(s, "[] \n\r");
//    string s = "%$#@///127.0.0.1///%$#@";
//    s = Str::trim(s, "%$#@/");
//    std::cout << "|" << s << "|" << std::endl;

// check to_str() work properly
//    string s = Str::to_str("%s - %s:%d", "ERROR: data type is illegel", __FILE__, __LINE__);
//    std::cout << s << std::endl;

// check contain() work properly
//    string s = "hello world";
//    std::cout << Str::contain(s, "a") << std::endl;
//    std::cout << Str::contain(s, "abc") << std::endl;
//    std::cout << Str::contain(s, "hello") << std::endl;
//    std::cout << Str::contain(s, " ") << std::endl;

    return 0;
}
