#include <iostream>
#include <functional>

#include <reflect/class_factory.h>

#include <test/a.h>

using namespace demo::reflect;





//class Test
//{
//public:
//    Test() = default;
//    ~Test() = default;
//
//    void foo()
//    {
//        std::cout << "Test::foo" << std::endl;
//    }
//};
//
//typedef std::function<void(Test *)> test_method;





int main()
{
//    auto factory = Singleton<ClassFactory>::instance();
//    Object * a = factory->create_class("A");

//    string name;
//    a->get<string>("m_name", name);
//    std::cout << name << std::endl;
//
//    int age = 0;
//    a->get<int>("m_age", age);
//    std::cout << age << std::endl;
//
//    a->set<string>("m_name", "kitty");
//    a->set<int>("m_age", 30);
//
//    a->get<string>("m_name", name);
//    std::cout << name << std::endl;
//
//    a->get<int>("m_age", age);
//    std::cout << age << std::endl;





//    test_method method = &Test::foo;
//
//    Test t;
////    method(&t);
//
//    uintptr_t ptr = (uintptr_t) &method;
//    (*(test_method *) (ptr))(&t);





    auto factory = Singleton<ClassFactory>::instance();
    Object * a = factory->create_class("A");

    a->call("f1");
    const string & name = "kitty";
    int age = 18;
    int ret = a->call<int>("f2", name, age);        // must define variables `name`, `age`
    std::cout << ret << std::endl;                  // because it only accept r-values

    return 0;
}
