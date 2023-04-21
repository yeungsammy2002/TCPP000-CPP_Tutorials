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
Note that `std::new_handler`, `std::set_new_handler()` and `std::bad_alloc` are defined in `<new>`.

This is the simplified version of operator `new` that I used to demonstrate what the standard operator `new` typically does. Before going to the details, let me introduce `std::new_handler`. `std::new_handler` is a function that invoked when operator `new` failed to allocate memory. By default, a `new` handler is a ***null pointer***, which means there is no `std::new_handler`. But you can always set a `std::new_handler` with the `std::set_new_handler()` function. `std::set_new_handler()` function not only installs a `std::new_handler`, it also returns an old `std::new_handler`, or the current `std::new_handler`. `operator new()` takes one parameter, which the `size` of the memory to be allocated, and it throws a `std::bad_alloc` exception. Inside the `operator new()`, there is an infinity loop. Inside of the loop, the first thing it does is to allocate the memory of this `size`. If the memory allocation is successful, it returns the memory and it's done, mission complele. If the memory allocation is not successful, it will fetch the current `std::new_handler` `Handler` and check if the `Handler` is ***null***. If it is not ***null***, invoke the `std::new_handler` function `(*Handler)()`. After that, go back to the loop and try to allocate memory again:
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
        throw std::bad_alloc();                      // If new handler is null, throw exception
}
```
So you might have gussed, the purpose of the `std::new_handler` `Handler` is to make more memory available so that next run of memory allocation could be successful. If the `Handler` is a ***null pointer***, which means there is nothing I can do to free the memory. Then it will just throw a `std::bad_alloc()` exception. These are the things that standard operator `new` typically does. You can overload the operator `new` to do whatever things that you want to do, but it is generally a good practice to follow the same format of the standard operator `new`, such as a `while`-loop, a `std::bad_alloc()` exception.


### Memmber Operator `new`
You may have notice that in the previous example we are defining a **global operator `new`**. Sometimes you don't want to overload the **global operator `new`**, you only want to overload the **operator `new`** for your own class. 

Here we are defining a **member operator `new`** for our class `Dog`.The **member operator `new`** also takes one parameter of `std::size_t`, and also throw a `std::bad_alloc` exception. Inside the operator `new`, I call a function `customNewForDog()`, which create this `size` of the memory for `Dog`. Now I have a `YellowDog`, which is derived from `Dog`. In the `main()` function, I create new `YellowDog` on the ***heap***. Now do you smell any problem from this code?
```
class Dog {
    ...
public:
    static void* operator new(std::size_t size) throw(std::bad_alloc) {
        customNewForDog(size);
    }
};

class YellowDog : public Dog {
    int age;
};

int main() {
    YellowDog* py = new YellowDog();
}
```
The `Dog`'s operator `new` is a public method, that means it will be inherited by its child, `YellowDog`. So when I create a `YellowDog` on the ***heap***. It will actually call the `Dog`'s operator `new`, not the standard operator `new`. As a result, I will be calling `customeNewForDog()` to allocate a memory for `YellowDog`, which may or may not be what I have in mind.

Suppose I don't want that, what can I do? Creating a new `YellowDog` invokes the `Dog`'s operator `new`. However, what parameter will be positive to the `Dog`'s operator `new`? Will be the `Dog`'s `size`? Or the `YellowDog`'s `size`? Fortunately, it's the `YellowDog`'s `size`. So inside the `Dog`, operator `new` for `Dog`. I could do some check based on that **\*here**. If the `size` is equals to `Dog`'s `size`, I will call `customNewForDog()`. Otherwise, I will call the standard operator `new` for `YellowDog`:
```
class Dog {
    ...
public:
    static void* operator new(std::size_t size) throw(std::bad_alloc) {
        if(size == sizeof(Dog))                 // *here
            customNewForDog(size);
        else
            ::operator new(size);               // call the standard operator `new` for `YellowDog`
    }
    ...
};

class YellowDog : public Dog {
    int age;
};

int main() {
    YellowDog* py = new YellowDog();
}
```
So this is one solution.

Here is another solution. Since I have defined a customize operator `new` for `Dog`, maybe I could define a customize operator `new` for `YellowDog` too:
```
class Dog {
    ...
public:
    static void* operator new(std::size_t size) throw(std::bad_alloc) {
        if(size == sizeof(Dog))                 // *here
            customNewForDog(size);
        else
            ::operator new(size);               // call the standard operator `new` for `YellowDog`
    }
    ...
};

class YellowDog : public Dog {
    int age;
    static void* operator new(std::size_t size) throw(std::bad_alloc) {
        ...
    }
};

int main() {
    YellowDog* py = new YellowDog();
}
```
So this is the second solution.


### Similarly for operator `delete`
Now let's look at the operator `delete`. operator `delete` expect one parameter, which is a pointer to the memory to be deleted `pMemory`, and it is not suppose to throw any exception. **\*Here** I have overload the operator `delete` for `Dog`, and **\*here2** overload the operator `delete` for `YellowDog`. So the problem of the previous example will not exist here. In the `main()` function, I create a new `YellowDog` on the ***heap***, and assigns a pointer to a `Dog`'s pointer `pd`. Later on, I delete `pd`. Do you see any problem with this code?
```
class Dog {                                                 // *Here
    static void operator delete(void* pMemory) throw() {
        std::cout << "Bo is deleting a dog, \n";
        customDeleteForDog();
        free(pMemory);
    }
};

class YellowDog : public Dog {
    static void operator delete(void* pMemory) throw() {
        std::cout << "Bo is deleting a yellowdog, \n";
        customDeleteForYellowDog();
        free(pMemory);
    }
};

int main() {
    Dog* pd = new YellowDog();
    delete pd;
}
```
When I delete `pd`, which operator `delete` will be invoked? It's the `Dog`'s operator `delete` will be invoked. So I end up using the `Dog`'s operator `delete` to deallocate the memory that is being allocated for `YellowDog`, which smell trouble. So what can I do? You may immediately suggest that making the operator `delete` a virtual method. Then the correct operator should be invoked, right?
```
class Dog {
    virtual static void operator delete(void* pMemory) throw() {
        std::cout << "Bo is deleting a dog, \n";
        customDeleteForDog();
        free(pMemory);
    }
};
...
```
This is certainly will **NOT** work. You cannot define a method to be both `virtual` and `static`, because a method being `static` specify the behavior of the `class`. It is not directly tied to the object. And a method being `virtual` specify the behavior of the object. So `static` and `virtual` belongs to different world. That is why it is forbidden in C++ to define a method to be both `virtual` and `static`. What is our solution?

If you remember what previous section we have talked about, if a `class` is meant to be used polymorphically, it should have a ***virtaul destructor***. That rules applied here also. We need to define a ***virutal destructor*** even though the ***virtual destructor*** doesn't do anything:
```
class Dog {
    static void operator delete(void* pMemory) throw() {
        std::cout << "Bo is deleting a dog, \n";
        customDeleteForDog();
        free(pMemory);
    }
    ~Dog() {}
}
...
```
Now when I delete `pd`, it will invoke the `YellowDog`'s destructor, and magically invoke the `YellowDog`'s operator `delete`.


### Customize `new` & `delete`
Now let's talk about why and when we want to customize `new` and `delete`. Here I have listed some of the scenarios when you want to customize `new` and `delete`, but this is far from complete list:

1. ***Usage Error Detection***
    - ***Memory Leak Detection/Garbage Collection*** - If I keep in the operator `new`. I keep a record of all the memory being allocated, and in the operator `delete`, I remove the memory from the record. I will have a list of the memory being leak. And possibly, I can also implement my own ***garbage collection mechanism***.
    - ***Array Index Overrun/Underrun*** - In the operator `new`, if I assign a special signature to the first item of an array and another signature to the last item of the array, then I will be able to detect array overrun and underrun.

2. ***Improve Efficiency***
    - ***Clustering related objects to reduce page fault*** - I could clustering related objects to the same place and to reduce page fault. 
    - ***Fixed Size Allocation*** - I could use fixed size allocation, which is good for application with many small objects.
    - ***Align similar size objects to same places to reduce fragmentation***

3. ***Perform Additional Tasks***
    - ***Fill the deallocated memory with `0`'s - security*** - If my program is dealling with high security information, it is a good idea to fill the deallocated memory with zeros to prevent those information to be leaked to the outside world.
    - ***Collect Usage Statistics*** - I could use the operator `new` to collect usage statistics of the memory. For example, what's the average size of dynamically allocated memory, what's the biggest size of the memory, how often the memory allocation is performed...etc


### Writing a Good Manager is Hard
The last thing to keep in mind is it is easy to write an operator `new` and an operator `delete` that works, but it is very hard to write a good memory manager. So before you start off writing your own version of `new` and `delete`, there are two alternative you can consider:
1. **Tweak you compiler toward your needs** - Read your compiler's document carefully, and see if there is something you can use that the compiler already provided.
2. **Search for memory management library** - There are many good memory management library out there, for example, the ***Pool library*** from ***Boost***.

If these two alternatives doesn't work for you, then it's a time for your own version of the `new` and `delete`.




# Section 29 - How to Define New Handler

```
void* operator new(std::size_t size) throw(std::bad_alloc) {
    while(true) {
        void* pMem = malloc(size);
        if(pMem)
            return pMem;

        new_handler Handler = set_new_header(0);
        set_new_handler(Handler);

        if(Handler)
            (*Handler)();
        else
            throw bad_alloc();
    }
}
```
