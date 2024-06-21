#include <endian.h>
#include <iostream>

int main(int argc, const char * argv[])
{
    if (__BYTE_ORDER == __LITTLE_ENDIAN)
    {
        printf("LITTLE ENDIAN\n");
    } else if (__BYTE_ORDER == __BIG_ENDIAN)
    {
        printf("BIG ENDIAN\n");
    } else
    {
        printf("UNKNOW\n");
    }
}