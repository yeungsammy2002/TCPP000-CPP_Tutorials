#include <iostream>

#include <serialize/datastream.h>

using namespace demo::serialize;

int main()
{
//    std::cout << "serialize demo" << std::endl;

    Datastream ds;

//    ds.write(true);
//    ds.write(123);
//    ds.write(1.23);
//    ds.write("hello world");
//    ds.show();

    ds << true;
    ds << 123;
    ds << 1.23;
    ds << "hello world";
    ds.show();

    bool b;
    int i;
    double d;
    string s;

//    ds.read(b);
//    ds.read(i);
//    ds.read(d);
//    ds.read(s);

    ds >> b >> i >> d >> s;

    std::cout << std::boolalpha;
    std::cout << b << " " << i << " " << d << " " << s;

    return 0;
}