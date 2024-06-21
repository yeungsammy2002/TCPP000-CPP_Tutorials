#include <cstring>
#include <iostream>

int main()
{
    int n = 0x12345678;
    char str[4];
    memcpy(str, &n, sizeof(int));
    
//    for (int i = 0; i < sizeof(int); ++i)
//    {
//        printf("%x\n", str[i]);
//    }

    if (str[0] == 0x12)
    {
        printf("BIG ENDIAN\n");
    } else if (str[0] == 0x78)
    {
        printf("LITTLE ENDIAN\n");
    } else
    {
        printf("UNKNOWN\n");
    }
}