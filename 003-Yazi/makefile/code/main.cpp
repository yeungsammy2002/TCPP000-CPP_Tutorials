#include <iostream>

#include <add.h>
#include <sub.h>
#include <mul.h>

int main()
{
    int res = add(1, 2);
    std::cout << "1 + 2 = " << res << std::endl;

    res = sub(9, 1);
    std::cout << "9 - 1 = " << res << std::endl;

    res = mul(3, 5);
    std::cout << "3 * 5 = " << res << std::endl;

    return 0;
}