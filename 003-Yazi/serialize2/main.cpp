#include <iostream>

#include <serialize/data_stream.h>
#include <serialize/serializable.h>

using namespace demo::serialize;





//class A : public Serializable
//{
//public:
//    A() = default;
//    A(const string & name, int age) : m_name(name), m_age(age) {}
//    ~A() = default;
//
//    virtual void serialize(DataStream & stream) const
//    {
//        char type = DataStream::CUSTOM;
//        stream.write((char *) &type, sizeof(char));
//        stream.write(m_name);
//        stream.write(m_age);
//    }
//
//    virtual bool deserialize(DataStream & stream)
//    {
//        char type;
//        stream.read((char *) &type, sizeof(char));
//        if (DataStream::CUSTOM != type)
//        {
//            return false;
//        }
//        stream.read(m_name);
//        stream.read(m_age);
//        return true;
//    }
//
//    void show() const
//    {
//        std::cout << "name=" << m_name << ",age=" << m_age << std::endl;
//    }
//
//private:
//    string m_name;
//    int m_age;
//};





class A : public Serializable
{
public:
    SERIALIZE(m_name, m_age);

public:
    A() = default;
    A(const string & name, int age) : m_name(name), m_age(age) {}
    ~A() = default;

    void show() const
    {
        std::cout << "name=" << m_name << ",age=" << m_age << std::endl;
    }

private:
    string m_name;
    int m_age;
};





int main()
{
    DataStream ds;





//    ds.write(true);
//    ds.write(123);
//    ds.write("hello world");
//    ds.show();
//
//    bool b;
//    int i;
//    string s;
//
//    ds.read(b);
//    ds.read(i);
//    ds.read(s);
//
//    std::cout << "b=" << b << ",i=" << i << ",s=" << s << std::endl;




//    ds << true << 123 << "hello world";
//    bool b;
//    int i;
//    string s;
//
//    ds.read(b);
//    ds.read(i);
//    ds.read(s);
//
//    std::cout << "b=" << b << ",i=" << i << ",s=" << s << std::endl;





//    ds << true << 123 << "hello world";
//    bool b;
//    int i;
//    string s;
//
//    ds >> b >> i >> s;
//
//    std::cout << "b=" << b << ",i=" << i << ",s=" << s << std::endl;





//    std::vector<int> v{ 1, 2, 3 };
//
//    ds.write(v);
//
//    std::vector<int> v2;
//    ds.read(v2);
//
//    for (auto a: v2)
//    {
//        std::cout << a << std::endl;
//    }





//    std::vector<int> v{ 1, 2, 3 };
//    ds << v;
//
//    std::vector<int> v2;
//    ds >> v2;
//
//    for (auto a: v2)
//    {
//        std::cout << a << std::endl;
//    }





//    A a("jack", 18);
//    a.serialize(ds);
//
//    A a2;
//    a2.deserialize(ds);
//    a2.show();





//    A a("jack", 18);
//
//    ds << a;
//
//    A a2;
//    ds >> a2;
//
//    a2.show();





//    A a("jack", 18);
//    ds << a;
//    ds.save("./../a.out");





//    ds.load("./../a.out");
//    A a2;
//    ds >> a2;
//    a2.show();





    return 0;
}
