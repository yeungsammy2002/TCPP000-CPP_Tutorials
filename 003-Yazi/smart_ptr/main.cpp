#include <iostream>
#include <string>
#include <memory>

#include <utilities/smart_ptr.h>
#include <utilities/auto_ptr.h>
#include <utilities/unique_ptr.h>
#include <utilities/shared_ptr.h>

#include "test/a.h"
#include "test/b.h"

using namespace demo::utilities;

using std::string;

class Test
{
public:
    Test() : m_name("") {}

    ~Test()
    {
        std::cout << "Test is deleted" << std::endl;
    }

    void name(const string & name)
    {
        m_name = name;
    }

    string name() const
    {
        return m_name;
    }

private:
    string m_name;
};

int main()
{
//    std::cout << "Smart Pointer demo" << std::endl;

//    SmartPtr<int> p(new int(0));

    Test * p = new Test();

//    SmartPtr<Test> sp(p);
//    sp->name("jack");
//    std::cout << sp->name() << std::endl;


//    AutoPtr<Test> ap(p);
//    ap->name("jack");

//    std::cout << ap->name() << std::endl;

//    AutoPtr<Test> ap2(ap);
//    std::cout << ap2->name() << std::endl;
//    std::cout << ap->name() << std::endl;

//    AutoPtr<Test> ap2;
//    ap2 = ap;
//    std::cout << ap2->name() << std::endl;
//    std::cout << ap->name() << std::endl;

//    UniquePtr<Test> up(p);
//    up->name("jack");
//    std::cout << up->name() << std::endl;

//    UniquePtr<Test> up2(up);
//    std::cout << up2->name() << std::endl;

//    UniquePtr<Test> up2;
//    up2 = up;
//    std::cout << up2->name() << std::endl;

//    UniquePtr<Test> up2(std::move(up));
//    std::cout << up2->name() << std::endl;

//    UniquePtr<Test> up2;
//    up2 = std::move(up);
//    std::cout << up2->name() << std::endl;

//    SharedPtr<Test> sp(p);
//    sp->name("jack");
////    std::cout << sp->name() << std::endl;
//    std::cout << sp.use_count() << std::endl;

//    SharedPtr<Test> sp2(sp);
//    std::cout << sp2->name() << std::endl;
//    std::cout << sp.use_count() << std::endl;

//    SharedPtr<Test> sp2;
//    sp2 = sp;
//    std::cout << sp2->name() << std::endl;
//    std::cout << sp.use_count() << std::endl;

//    std::shared_ptr<A> a(new A());
//    std::shared_ptr<B> b(new B());
//    std::cout << "a count: " << a.use_count() << std::endl;
//    std::cout << "b count: " << b.use_count() << std::endl;
//    a->m_b = b;
//    b->m_a = a;
//    std::cout << "a count: " << a.use_count() << std::endl;
//    std::cout << "b count: " << b.use_count() << std::endl;

    SharedPtr<A> a(new A());
    SharedPtr<B> b(new B());
    std::cout << "a count: " << a.use_count() << std::endl;
    std::cout << "b count: " << b.use_count() << std::endl;
    a->m_b = b;
    b->m_a = a;
    std::cout << "a count: " << a.use_count() << std::endl;
    std::cout << "b count: " << b.use_count() << std::endl;

    return 0;
}
