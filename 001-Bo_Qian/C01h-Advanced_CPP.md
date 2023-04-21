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
2. Vice-versa, functions that are part of `C`'s interface should be in the same namespace as `C`. Say I have defined an `C` object `c`. Since the syntax said I `C`'s member method without using a qualifier, I should also be able to invoke a non-member function that operate on `C` without a qualifier. If that function comes from the same namespace as `C`, because both functions belongs to the interface of `C`. This is the theroetical reason behind ***Koening lookup***:
```
A::C c;
c.f();
h(c);
```

Now suppose I'm a little suspicious about this principle. What will happen if I have a non member function that should belongs to this interface but not in the same namespace as `C`? Will it bite me?

Let's look at an example. We have a namespace `A`. Inside `A`, we have a class `C`. Then we define an `operator+()` that works on `C` **\*here**. This operator really should belongs to the interface of `C`, but it is not in the same namespace as `C`. In the `main()` function, I create an array of `C`, and then call the standard library function `std::accumulate()` on the array `arr`:
```
namespace A {
    class C {};
}

int operator+(A::C, int n) {                // *here
    return n + 1;
}

int main() {
    A::C arr[3];
    std::accumulate(arr, arr + 3, 0);       // return 3
}
```

And here is the definition of the function `std::accumulate()`. You can ignore most of the function. What's important to us is belongs to namespace `std`, and the `std::accumulate()` function will invoke the `operator+` **\*here2**. Since we've already define `operator+` for `C`, apparently this is what we want to use. Now the question is when the compiler see the operator `+` **\*here2**. Can it find our `operator+` successfully **\*here**?
```
// Defined in C++ standard library <numeric>
namespace std {
    template <class InputIterator, class T>
        T accumlate(InputIterator first, InputIterator last, T init) {
            while(first != last)
                init = init + *first++;         // *here2
            return init;
        }
}
```
The answer is probably **NOT**. It depends on what header files you have included. Remember the ***name hiding* rule**? When the compiler see the operator `+` **\*here2** , it will first search the `operator+` in the ***current scope***. If it cannot find one, it will go to the ***global scope*** and search for it. 

However, if the compiler did find the `operator+` in current scope, regardless of the types of the parameter that `+` is taking, it will stop searching. That is really bad, because there are bunch of `operator+` that's defined in the namespace `std`. And then you could easily include some header files, and our own `operator+` **\*here** is hidden.So as you can see, **\*here** could indeed bite me.

The solution is follow the principle, and put the `operator+` in this same namespace as `C` **\*here**. Now the compiler will be able to our `operator+`  because of the ***Koening lookup***:
```
namespace A {
    class C {};
    int operator+(A::C, int n) {            // *here
        return n + 1;
    }
}

int main() {
    A::C arr[3];
    std::accumulate(arr, arr + 3, 0);
}
```
This is why we need to remember the principle and apply them during our daily coding.




# Section 28 - Demystifying Operator `new`/`delete`
We're going to talk about **operator `new`** and **operator `delete`**. What do they do? And how to overload them?

### `new` Operator
Let's start with the basic. I'm create a `Dog` on the ***heap***. What would happen as a result of this code?
```
Dog* pd = new Dog();
```
There are 3 things that would happen, and they happened in the fix order.
- **Step 1.** operator `new` is called to allocate memory for `Dog` object.
  
- **Step 2.** `Dog`'s constructor is called to create `Dog`.
  
- **Step 3.** If ***Step 2***, `Dog`'s constructor, throw an exception, call operator `delete` to free the memory allocated in ***Step 1***. However, if the ***Step 1***, operator `new`, throws an exception, the operator `delete` will not be invoked to free the memory, because C++ will assume the allocation of memory has not been successfully. So you need to keep that in mind when you write your own operator `new`.


### `delete` Operator
```
delete pd;
```
`delete pd;` will do the opposite thing:
- **Step 1.** The `Dog`'s destructor is called to destroy `Dog`.
  
- **Step 2.** operator `delete` is called to free the memory.


### Simplified Version of Operator `new`
Note that `std::new_handler` and `std::set_new_handler()` are defined in `<new>`.

This is the simplified version of operator `new` that I used to demonstrate what the standard operator `new` typically does. Before going to the details, let me introduce `std::new_handler`. `std::new_handler` is a function that invoked when operator `new` failed to allocate memory. By default, a `new` handler is a ***null pointer***, which means there is no `std::new_handler`. But you can always set a `std::new_handler` with the `std::set_new_handler()` function. `std::set_new_handler()` function not only installs a `std::new_handler`, it also returns an old `std::new_handler`, or the current `std::new_handler`. `operator new()` takes one parameter, which the `size` of the memory to be allocated, and it throws a `std::bad_alloc` exception. Inside the `operator new()`, there is an infinity loop. Inside of the loop, the first thing it does is to allocate the memory of this `size`. If the memory allocation is successful, it returns the memory and it's done. Mission complele:
```
void* operator new(std::size_t size) throw(std::bad_alloc) {
    while(true) {
        void* pMem = malloc(size);              // Allocate memory
        if(pMen)
            return pMem;                        // Return the memory if successful
    }

    std::new_handler Handler = std::set_new_handler(0);     // Get new handler
    std::set_new_handler(Handler);

    if(Handle)
        (*Handler)();                           // Invoke new handler
    else
        throw bad_alloc();                      // If new handler is null, throw exception
}
```

# 28 - 2:50

