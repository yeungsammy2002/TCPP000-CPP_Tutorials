#include <iostream>

#include <utilities/string.h>

using namespace demo::utilities;

int main()
{
//    std::cout << String::to_lower("ABC") << std::endl;
//    std::cout << String::to_upper("hello world") << std::endl;

//    std::cout << String::ltrim("ABC", "BA") << std::endl;
//    std::cout << String::rtrim("hello world", "drl") << std::endl;
//    std::cout << String::trim("hello world", "drlh") << std::endl;

////    const string & input = "/root/a/b/c";
//    const string & input = "root/a/b/c";
//    std::vector<string> output = String::split(input, '/');
//    for (auto & s: output)
//    {
//        std::cout << s << std::endl;
//    }

//    std::vector<string> input;
//    input.push_back("a");
//    input.push_back("b");
//    input.push_back("c");
//    std::cout << String::join(input, '-') << std::endl;

//    std::cout << String::capitalize("hello") << std::endl;

    std::cout << String::format("name=%s age=%d", "jack", 18) << std::endl;

    return 0;
}
