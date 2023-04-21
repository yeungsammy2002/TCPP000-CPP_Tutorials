# Section 27 - Koening Lookup and Namespace Design
Last time, we've talked about ***Koening lookup***. ***Koening lookup*** is for the purpose of **function name search**. It temporary extend the scope of function name search to the space where the function parameter type is defined. So in this case, it will extend the name search scope for `g(x1)` to the space where `X` is defined:
```
namespace A {
    struct X {};
    void g(X) {
        std::cout << "calling A::g() \n";
    }
}

int main() {
    A::X x1;
    g(x1);      // Koening lookup, or Argument Dependent Lookup (ADL)
}
```
Is this really a good thing for us? Doesn't that defeat the purpose of namespace and expose us to the danger of name crash? Another thing is if I have another function `g()` **\*here1**, which also in namespace `A`, but it doesn't take parameter. And then in `main()` function, I call this function `g()` **\*here3**. This appearly will not work. This code **\*here2** will succeed, this code **\*here3** will fail. And they both under the same context and calling a function from the same namespace `A`. Doesn't that looking a little weird?
```
namespace A {
    struct X {};
    void g(X) {
        std::cout << "calling A::g() \n";
    }
    void g() {                              // *here1
        std::cout << "calling A::g() \n";
    }
}

int main() {
    A::X x1;
    g(x1);                                  // *here2
    g();                                    // *here3, compiler error
}
```
It is a little weird if you are not used to viewing parameter as a way of expanding the looking up scope for function. Nevertheless, ***Koening lookup*** is good thing to have, and there are two main reason behind that, one is a ***practical reason***, and another one is a ***theoretical reason***.


- ### Practical Reason
This is the code that we use very often, it insert some message to the `std::cout`:
```
std::cout << "Hi.\n";           // calls `std::operator<<`
```
However, the reason this code can work is because of ***Koening lookup***. Although in the code we only have one **`std` qualifier**, there are two things in the code that comes from the `std` namespace. One is the `cout`, another one is the left shifter `<<`. So if we don't have ***Koening lookup***, we'll end up having something like this:
```
std::cout std::<< "Hi.\n";      // calls `std::operator<<`
```
This looks so weird. And even worse, this code won't even compile, in order for it to compile, you have to do something like this:
```
std::operator<<(std::cout, "Hi.\n");
```
This is even uglier than previous one. So having shown you the alternatives I imagine, you have started appreciating ***Koening lookup***.

***Koening lookup*** make it easier to mimic the behavior of operator that's provided by ***C++ core language***. Therefore, it makes you code cleaner.


- ### Theoretical Reason
Let me ask you a question. What is the interface of a class? 

We have a namespace `A`. And inside `A`, we have a class `C`. What is the interface of `C`?
```
namespace A {
    class C {
    public:
        void f() = 0;
        void g() = 0;
    };
}
```
Apparently, all the public methods that define inside class `C` are part of the interface of `C`. However, if I have another function called `h()`, which operate on `C`. The function `h()` belongs to interface of `C`?
```
namespace A {
    class C {
    public:
        void f() = 0;
        void g() = 0;
    };
    void h(C);
}
```
If you are hesitating, let me give you another one. The left shift operator `<<`, doesn't this guy looks like the interface of `C`?
```
namespace A {
    class C {
    public:
        void f() = 0;
        void g() = 0;
    };
    void h(C);
    ostream& operator<<(std::ostream&, const C&);
}
```
I would argue both of them should be part of the interface of `C`.

### Definition of `class`
Let's look at the definition of a `class`.

A `class` describes a set of data, along with the functions that operate on that data. 

So the definition didn't say along with the member methods that operator on the data, it only says functions. So by definition, the function `h()` and the left shift operator `<<` are not excluded from the class. If you really think about it, from `C`'s client point of view, there is no fundamental difference between using function `f()` and function `h()`. In either case, they are using class `C`, and calling some function that operates on class `C`.

Now suppose I have another function `j()`, which also operates on `C`, but it is outside of the namespace `A`. Will you call function `j()` a part of `C`'s interface? Apparently not. `j()` looks very much like a client function that operates on class `C`:
```
namespace A {
    class C {
    public:
        void f() = 0;
        void g() = 0;
    };
    void h(C);
    ostream& operator<<(std::ostream&, const C&);
}

void j(C);
```
So with that, I'm introducing an important ***engineering principle***.


### Engineering Principle
The principle says:
1. Functions that operate on class `C`, and in a same namespace with `C` are part of `C`'s interface.
2. Vice-versa, functions that are part of `C`'s interface should be in the same namespace as `C`. Say I have defined an `C` object `c`. Since the syntax said I `C`'s member method without using a qualifier, I should also be able to invoke a non-member function that operate on `C` without a qualifier. If that function comes from the same namespace as `C`, because both functions belongs to the interface of `C`. This is the ?? reason behind ***Koening lookup***
```
A::C c;
c.f();
h(c);
```
