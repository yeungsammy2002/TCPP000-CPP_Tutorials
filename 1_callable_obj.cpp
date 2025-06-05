#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include <string>
#include <functional>

using std::string;






void show(int num, const string & name)
{
	std::cout << "show: " << num << ", " << name << std::endl;
}

using T1 = void(int, const string &);

T1 t1;
void t1(int num, const string & name)
{
	std::cout << "t1: " << num << ", " << name << std::endl;
}

using T1_ptr = void(*)(int, const string &);
T1_ptr t1_ptr = show;
T1_ptr t1_ptr2 = &show;

typedef void(*TD1_ptr)(int, const string &);
TD1_ptr td1_ptr = show;
TD1_ptr td1_ptr2 = &show;

std::function<void(int, const string &)> f1 = show;
std::function<void(int, const string &)> fa1 = &show;

std::function<void(int)> fb1 = std::bind(show, std::placeholders::_1, "chris");






class A
{
public:
	static void show(int num, const string & name)
	{
		std::cout << "A show: " << num << ", " << name << std::endl;
	}

	using T2 = void(int, const string &);
	static T2 t2;
};

void A::t2(int num, const string & name)
{
	std::cout << "t2: " << num << ", " << name << std::endl;
}

using T2_ptr = void(*)(int, const string &);
T2_ptr t2_ptr = A::show;
T2_ptr t2_ptr2 = &A::show;

typedef void(*TD2_ptr)(int, const string &);
TD2_ptr td2_ptr = A::show;
TD2_ptr td2_ptr2 = &A::show;

std::function<void(int)> fb2 = std::bind(A::show, std::placeholders::_1, "john");






auto lamda1 = [](int num, const string & name) {
	std::cout << "lamda1: " << num << ", " << name << std::endl;
	};

using T3 = decltype(lamda1);						// the type of each lamba object is unqiue
T3 t3 = lamda1;										// `t3` can only initialized by using the original object `lamda1`

using T3_ptr = void(*)(int num, const string &);
T3_ptr t3_ptr = lamda1;								// cannot assign address of lambda object to the ptr
													// i.e. T3_ptr t3_ptr = &lamda1;  -- not legit
typedef void(*TD3_ptr)(int, const string &);
TD3_ptr td3_ptr = lamda1;

std::function<void(int)> fb3 = std::bind([](int num, const string & name) {
	std::cout << "lambda wrapper: " << num << ", " << name << std::endl;
	}, std::placeholders::_1, "sam");







class AA
{
public:
	void operator()(int num, const string & name)
	{
		std::cout << "functor: " << num << ", " << name << std::endl;
	}
};

// no alias of type of functor is allowed
// no alias of pointer type which points to functor type is allowed

std::function<void(int)> fb4 = std::bind(AA(), std::placeholders::_1, "leo");









class AAA
{
public:
	using T5_ptr = void(*)(int, const string &);
	operator T5_ptr()
	{
		return show;
	}
};

// no alias of type of type conversion operator overloading function is allowed
// no alias of pointer type which points to type conversion operator overloading function type is allowed

std::function<void(int)> fb5 = std::bind(AAA(), std::placeholders::_1, "boris");








class AAAA
{
public:
	void aaaa_show(int num, const string & name)
	{
		std::cout << "aaaa show: " << num << ", " << name << std::endl;
	}
};

using T6_ptr = void(AAAA:: *)(int, const string &);
T6_ptr t6_ptr = &AAAA::aaaa_show;						// only address of non-static member function is allowed to be assigned
														// i.e. T6_ptr t6_ptr = AAAA::aaaa_show;  -- not legit
typedef void(AAAA:: * TD6_ptr)(int, const string &);
TD6_ptr td6_ptr = &AAAA::aaaa_show;

AAAA aaaa;

std::function<void(int)> fb6 = std::bind(&AAAA::aaaa_show, &aaaa, std::placeholders::_1, "alex");
std::function <void(AAAA &, int, const string &)> fn6 = &AAAA::aaaa_show;






template<typename Fn, typename... Args>
auto callback(Fn && fn, Args && ...args) ->
decltype(std::bind(std::forward<Fn>(fn), std::forward<Args>(args)...))
{
	auto func = std::bind(std::forward<Fn>(fn), std::forward<Args>(args)...);
	func();
	return func;
}






int main()
{
	//t1(8, "chris");
	//t1_ptr(8, "chris");
	//t1_ptr2(8, "chris");
	//td1_ptr(8, "chris");
	//td1_ptr2(8, "chris");
	//f1(8, "chris");
	//fa1(8, "chris");
	//fb1(8);

	//A::show(88, "john");
	//A::t2(88, "john");
	//t2_ptr(88, "john");
	//t2_ptr2(88, "john");
	//td2_ptr(88, "john");
	//td2_ptr2(88, "john");
	//fb2(8);

	//lamda1(888, "sam");
	//t3(888, "sam");
	//t3_ptr(888, "sam");
	//td3_ptr(888, "sam");
	//fb3(888);

	//AA()(8888, "leo");
	//AA aa;
	//aa(8888, "leo");
	//fb4(8888);

	//AAA()(88888, "boris");
	//AAA aaa;
	//aaa(88888, "boris");
	//fb5(88888);

	//AAAA aaaa;
	//(aaaa.*t6_ptr)(888888, "alex");
	//(aaaa.*td6_ptr)(888888, "alex");
	//fb6(888888);
	//fn6(aaaa, 888888, "alex");

	auto fn = callback(show, 8, "chris");
	fn();

	return 0;
}