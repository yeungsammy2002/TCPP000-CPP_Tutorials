#include <iostream>

#include <utilities/string.h>

using namespace demo::utilities;

int main()
{
//    std::cout << String::to_lower("HELLO WORLD") << std::endl;
//    std::cout << String::to_upper("kitty") << std::endl;

//    std::cout << String::ltrim(" ___ hello world ___ ", "_ ") << std::endl;
//    std::cout << String::rtrim(" ___ hello world ___ ", "_ ") << std::endl;
//    std::cout << String::trim(" ___ hello world ___ ", "_ ") << std::endl;

//    const string & input = "/root/a/b/c";
//    const string & input = "/root/a/b/c/";
//////    const string & input = "root";
//////    const string & input = "/root/";
//    std::vector<string> output = String::split(input, "/");
//    std::vector<string> output = String::split(input, '/');
//    for (const auto & str: output)
//    {
//        std::cout << str << std::endl;
//    }

//    const string & input = "[ a, b, c, d ]";
//    std::vector<string> output = String::split(input, " ,[]");
//    for (const auto & str: output)
//    {
//        std::cout << str << " ";
//    }
//    std::cout << std::endl;

//    std::vector<string> input;
//    input.push_back("a");
//    input.push_back("b");
//    input.push_back("c");
//    std::cout << String::join(input, " , ") << std::endl;
////    std::cout << String::join(input, '-') << std::endl;

//    std::cout << String::capitalize("hello") << std::endl;
//    std::cout << String::capitalize("hello world") << std::endl;

    std::cout << String::format("name=%s age=%d", "jack", 18) << std::endl;

    return 0;
}