# Section 23 - Duality of Public Inheritance - Interface & Implementation
We're going to talk about the the ***duality of public inheritance***. This seemingly innocent public inheritance, actually has a duality in it, which is:
- the ***inheritance of interface***
- the ***inheritance of implementation***

A ***public inheritance*** could be either one of them, or both of them. Let's look at our example.

Our class `Dog` has a `bark()` method, and the `bark()` method is a ***pure virtual method***. And the `YellowDog`, which is derived from `Dog` overwrite the `bark()` method with its own implementation. In this case, the `YellowDog` inherit the interface of the `bark()` method and does not inherit the implementation of `bark()` method, because the `bark()` method doesn't have implementation anyway, it's a ***pure virtual method***. So this is an **interface only inheritance**.
```
class Dog {
public:
    virtual void bark() = 0;
};

class YellowDog : public Dog {
public:
    virtual void bark() {
        std::cout << "I am a yellow dog.\n";
    }
};
```

Now suppose we have another method. It's a regular method called `run()`, and of course, the `run()` method needs the implementation:
```
class Dog {
public:
    virtual void bark() = 0;
    void run() {                                    // here
        std::cout << "I am running." << std::endl;
    }
};

class YellowDog : public Dog {
public:
    virtual void bark() {
        std::cout << "I am a yellow dog.\n";
    }
};
```
As we discussed in other sections of maintaining ***"is-a" relationship***, a non virtual method should not be overwritten. So the `YellowDog` should not overwrite the `Dog`'s `run()` method. As a result, the `YellowDog` will inherit the interface of the `run()` method as well as the implementation of the `run()` method. So this is a ***dual inheritance*** of both ***interface*** and ***implementation***.

Now let's have another virtual method called `eat()`, and this virtual method has its own implementation. Now the `YellowDog` is give a choice, it can either overwrite the `eat()` method or not overwrite it. If it is choose not overwrite the `eat()` method, then `eat()` is just like `run()`, the `YellowDog` will inherit its interface as well as its implementation. If the `YellowDog` choose to overwrite the `eat()` method, then `eat()` is just like `bark()`. The `YellowDog` will inherit its interface, but not implementation. Instead, it will provide its own version of the implementation. So you can say a ***non pure virtual method*** is providing an **inheritance of the interface** as well as a **default implementation**:
```
class Dog {
public:
    virtual void bark() = 0;
    void run() {
        std::cout << "I am running.";
    }
    virtual void eat() {                            // here
        std::cout << "I am eating." << std::endl;
    }
};

class YellowDog : public Dog {
public:
    virtual void bark() {
        std::cout << "I am a yellow dog.\n";
    }
};
```

Now let's have another method, which is a protected method called `sleep()`, and it has its own implementation. Since the method is protected, the `YellowDog` will inherit the method. However, it only inherit the implementation of the method, and not the interface of the method. To provide a service for `sleep()`, the `YellowDog` needs to provide its own version of a ***public method***, let's call it `iSleep()`, and this method will call `sleep()`. Now the `YellowDog` has its own interface for `sleep()`:
```
class Dog {
public:
    virtual void bark() = 0;
    void run() {
        std::cout << "I am running.";
    }
    virtual void eat() {
        std::cout << "I am eating.";
    }
protected:
    void sleep() {
        std::cout << "I am sleeping.";
    }
};

class YellowDog : public Dog {
public:
    virtual void bark() {
        std::cout << "I am a yellow dog.\n";
    }
    void iSleep() {
        sleep();
    }
};
```
So as you can see, a protected method provides an ***implementation inheritance* only**.


### Types of Inheritance in C++ Summary
To summarize the types of inheritance provided by C++ language, there are four types:
1. ***Pure virtual public method*** - inherit interface only.
   
2. ***Non-virtual public method*** - inherit both interface and implementation.
   
3. ***Impure virtual public method*** - inherit interface and default implementation.
   
4. ***Protected method*** - inherit implementation only.

As a software designer who is design the software, it is very important to separate the concepts of interface and implementation. Because he needs to know when to use ***interface inheritance***, and when to use ***implementation inheritance***. Now the question for us is ***when***.


## Interface Inheritance
***Interface inheritance*** is the main feature of inheritance, the purpose of it is:
1. ***Subtyping*** - which means one type can be safely used in the context where another type is expected.
   
2. ***Polymorphism***

As you can see, these two are very important technique in object-oriented programming. And they are closely related. 

So a pure virtual method, which is a interface inheritance only, should be widely used when a ***subtyping*** and ***polymorphism*** is needed.


- ### Pitfulls of Interface Inheritance
- **Be careful of interface bloat** - Since the interface of a parent class is inherited by its child class, it can easily lead to interface boat. Interface should be design to be minimally enough. It provides enough functionality, but the size of it should be minimal.

- **Interface should not reveal implementation** - The whole purpose of interface design is to hide the implementation of details. So the implementation can change as much as it like, the interface should stay relatively fixed. If the interface reveals the implementation detail, then we are losing all the benefits of abstraction.


## Implementation Inheritance
***Implementation inheritance*** is generally not encourage, although sometimes we do need them. ***Implementation inheritance*** has many problems. One of the big problems is it increase code complexity. Think about the way we write code. We first load a chunk of code into our head, understand it, get things right, and then move on and load another chunk. The problem with ***implementation inheritance*** is it makes that chunk of code too big for our head. In order to get something right in the derived class, I have to have a good understanding of all its base classes. And in order to change something in the base class, I have to be very careful of not breaking anything in all its derived classes. So things become complicated. ***Non virtual method*** and ***impure virtual method*** inherit both ***interface*** and ***implementation***. However, the implementation inheritance should be considered as side benefit of the ***interface inheritance***. A protected method inherits implementation only, so it should not encourage in general. If there is some implementation needs to be shared by the base class and derived class, move that implementation to some helper class, and both base class and derived class should have accesss to that helper class. It is much more flexible that way.
```
public:
    void run() {
        std::cout << "I am running.";
    }
    virtual void eat() {
        std::cout << "I am eating.";
    }
protected:
    void sleep() {
        std::cout << "I am sleeping.";
    }
```


- ### Guidelines for Implementation Inheritance
1. Do not use ***inheritance*** for code reuse, use ***composition*** instead. ***Inheritance*** is not for code reuse, ***composition*** is.
   
2. Minimize the ***implementation*** in ***base classes***. ***Base classes*** should be thin with ***implementation***.
   
3. Minimize the ***level of hierarchies*** in ***implementation inheritance***.

So both number 2 and number 3 will reduce the number of ***implemenation inheritance***.


### Inheritance is Evil
Some people claim that inheritance is evil, and it should be avoided all together. I'm not convinced of that, but I do agree this statement - ***"Inheritance is often useful, but more often overused"*** (or I should say ***abused***). This is particularly true for ***implementation inheritance***. When ***implementation inheritance*** is used too much, it does make inheritance look like evil. But in the end, inheritance is just a tool for you to use. And like any other tools, when it is used inappropriately, it will hurt you. However, we cannot call a knife evil, because it is used inappropriately and cut your finger. If you can remember all the pitfalls and guideline. ***Inheritance*** in your code should not look like evil.




# Section 24 - Code Reuse - Inheritance vs Composition
***Software engineering*** is largely about ***code reuse***. In C++, there are mainly **two ways** of **code reuse**, one is ***inheritance***, and another one is ***composition***. So which one should we use? We had a light touch on this topic in previous sections. We're going to dive deeper into it.

Let's look at our example. We have a class `BaseDog`. `BullDog` and `ShepherdDog` are derived from the `BaseDog`. The `BaseDog` defines some common activities, and `BullDog` and `ShepherdDog` will call this common activities to perform more task. I have seen this kind of code structure in a lot of places, and honestly, I don't like it. The problem I have is the name `BaseDog`. Almost every C++ program that I have seen, there is always a class, or even a bunch of classes which are named `Base{{something}}`. With inheritance, the class name should symbolize the ***"is-a" relationship*** between the ***base class*** and the ***derived class***. In our example, the `BullDog` is a `BaseDog`, which doesn't make any sense, what is a `BaseDog` anyway. Besides, the name `BaseDog` is not extendable, what if later on I design to create a new class, and derive `BaseDog` from that new class, what should I name the new class? Should I name it `BaseBaseDog`? I guess the biggest problem I have with the name `BaseDog` is it refuse the implementation detail. It will reveals the fact that this class `BaseDog` is used to derive other classes. A class name is part of the interface, and when the interface reveals the implementation details, we're losing all the benefits of abstraction. However, this is not a problem if the `BaseDog` is only an internal class, and not expose on the interface. 
```
class BaseDog {
    ...common activities...
};

class BullDog : public BaseDog {
    ...call the common activities to perform more tasks...
};

class ShepherdDog : public BaseDog {
    ...call the common activities to perform more tasks...
};
```


### Code Reuse with Inheritance
So instead of call it `Base{{something}}` why don't we just call it `{{something}}`. Instead of calling it `BaseDog`, why don't we just call it `Dog`? `BullDog` is a kind of `Dog`, `ShepherdDog` is another kind of `Dog`. It makes much more sense this way. And it's extendable, a `Dog` can be derived from an `Animal`, `Animal` can be derived from `Organism`, so on and so forth:
```
class Dog {
    ...common activities...
};

class BullDog : public Dog {
    ...call the common activities to perform more tasks...
};

class ShepherdDog : public Dog {
    ...call the common activities to perform more tasks...
};
```
Some people may argue ***"Yeah, yeah, but these are names, there are cosmetic chanages"***. In software engineering, good naming is not cosmetic. Good naming is a critical element of a good software design. We should always strive for precise and self-explaining names for our classes, our functions and our variables.

The `Dog` defines some common activities. And `BullDog`, `ShepherdDog` reuse these common activites to perform more tasks. So this is an example of ***code reuse with inheritance***. And let's look at how we can achieve the same kind of code with composition.


### Code Reuse with Composition
We have a new class `ActivityManager`, and all the common activities that originally defined in `Dog` are moved to the `ActivityManager`. Every `Dog` have a pointer to the `ActivityManager` - `pActMngr`, and then they reuse the code of the common activities through the `pActMngr`. This is code reuse with ***composition structure***. How does it look comparing to the inheritance example. On the surface, it seems like the ***composition*** is worse, because it has more lines of code, and it introduces a new `class`. However, the ***composition*** is actually a much better structure for code reuse. Let's discuss why.
```
class ActivityManager {
    ...common activities...    
};

class Dog {
    ...
};

class BullDog : public Dog {
    ActivityManager* pActMngr;
    ...call the common activities through pActMngr...
};

class ShepherdDog : public Dog {
    ActivityManager* pActMngr;
    ...call teh common activites through pActMngr...
};
```


### Code Reuse - Composition is Better than Inheritance
1. ***Less Code Coupling*** - ***Composition*** has less code coupling between the reused code and reuser of the code. Inheritance implies more code coupling between the child class and the parent class. The reasons are:
   - The child class automatically inherits ALL the parent class's public members. So whether you like it or not, the child class's interface is bloated with the parent class's interface. However, with the ***composition***, you can pick the things that you want to use and bring that into your interface.
   - The child class can access parent class's protected members. In other words, it can access the parent's internal. So in a sense, the ***inheritance* breaks *encapsulation***. There are two big enemies on encapsulation in C++, number one is `friend`s, number two is ***inheritance***. So friends and children, biggest enemies in your privacy, it is a philosophy of life.

2. ***Dynamic Binding*** - The second reason why the ***composition*** is better than the ***inheritance*** is ***dynamic binding***. The ***inheritance relationship*** is bound at ***compile time***. Once you pass the ***compile time***, the relationship cannot be changed anymore. However, the ***composition relationship*** can be bound either at ***compile time*** or at ***runtime*** through the ***polymorphism*** and the ***dynamic binding***. Say we have an `OutdoorActivityManager` class and an `IndoorActivityManager` class, both of them are derived from the `ActivityManager` class:
   ```
   class OutdoorActivityManager : public ActivityManager { ... }
   class IndoorActivityManager : public ActivityManager { ... }
   ```
   Since each `Dog` has a pointer to `ActivityManager`, we can switch between `OutdoorActivityManager` and `IndoorActivityManager` easily during the ***runtime***.

3. ***Flexible Code Construct*** - Lastly, ***composition*** has more flexible code construct. Say you have many types of `Dog` and many types of `ActivityManager`. With ***composition***, we can arbitrary combination of the `Dog`s and the `ActivityManager`s. There is no limit on how you can combine the `Dog` with an `ActivityManager`. If you really want, one `Dog` can either multiple instances of `ActivityManager` at the same time. With inheritance, you can only have one instance of the parent at a time:
   ```
   Dog              ActivityManager

   BullDog          OutdoorActivityManager
   ShepherdDog      IndoorActivityManager
   ```

So combine those three - ***less code coupling***, ***dynamic binding*** and ***flexible code construct***. We declare the ***composition*** is a better structure for ***code reuse*** than ***inheritance***. ***Inheritance*** is good for other things, but **NOT** for ***code reuse***.




# Section 25 - Namespace and Keyword - `using`
We're going to talk about the ***namespace*** and the ***name lookup***. We'll start with some basics, and then move on to some more advanced stuff.

***C++ namespace*** has an important keyword - `using`. This keyword can be used in two ways. 
1. **`using` directive** - it is to bring all namespace members into current scope. So that you can use them without the qualifier in front of them. If I have using `namespace std`, I'm bring every name under `std` into current scope:
   ```
   using namespace std;
   ```
   
2. **`using` declaration** - **`using` declaration** itself can be used in two ways:
   - It brings one specific namespace member to current scope. So if I have using `std::cout`, after that I can use `cout` without the ***qualifier***:
        ```
        using std::cout;
        cout << "Hello world.\n";
        ```
   - It brings a member from base class to current class's scope. So this is working with `class`, **NOT *namespace***. **`using` directive** can only work with ***namespace***, **`using` declaration** can work with either ***namespace*** or **`class`**.


Now let's look at an example with `class`es in it. We have a class `B`, which has a public method `f()`. We have a class `D`, which is derived from `B`, and it has public methods `g()` and `h()`:
```
using namespace std;            // case 1: `using` directive in global scope
using std::cout;                // case 2: `using` declaration in global scope

class B {
public:
    void f(int a);
};

class D : private B {
public:
    void g() {
        using namespace std;    // case 1b: `using` directive in local scope
        cout << "From D: \n";
    }
    void h() {
        using std::cout;        // case 2b: `using` declaration in local scope
        cout << "From E: \n";
    }
    using B::f;                 // case 2c: `using` declaration for class member
};
```
**`using` directive** and **`using` declaration**, when working with namespaces, they can be used either under ***global scope*** like `case 1` & `case 2` or in the ***local function scope*** like `case 1b` & `case 2b`. 

`case 2c` is **`using` declaration for `class` member**. It brings the method `f()` from the parent class `B` into current class `D` with ***public access***. You might be wondering why do we need to do that? Isn't the method `f()` inherited by `D` already? The class `D` is ***privately*** derived from `B`, which means `B`'s public method becomes `D`'s private method. So `f()` is `D`'s private method.

If I don't use the **`using` declaration**, then I create a `D` object `d`, and call `d.(8)`. This code will not compile, because `f()` is a private method:
```
...
class B {
public:
    void f(int a);
};

class D : private B {
public:
    void g() {
        using namespace std;
        cout << "From D: \n";
    }
    void h() {
        using std::cout;
        cout << "From E: \n";
    }
};

int main() {
    D d;
    d.f(8);             // Compiler error, f() is private method
}
```

However, after using the **`using`  declaration for method `f()`**, `d.f(8)` will compile:
```
...
class D : private B {
public:
    void g() {
        using namespace std;
        cout << "From D: \n";
    }
    void h() {
        using std::cout;
        cout << "From E: \n";
    }
    using B::f;         // `using` declaration for f()
};

int main() {
    D d;
    d.f(8);             // OK
}
```
You may have notice the **`using` declaration for class member** is used in **`class` scope**. It cannot be used in a ***local scope***, it cannot be used in a ***global scope***. So `using B::f` can only be used in ***class scope***.

So if I have a **`using` declaration of a class member** in a ***global scope***, this is illegal:
```
...
/* global scope */
using B::f;             // illegal
```

Similarly, if I have an **`using` declaration of namespace member** in a ***class scope***, this is also illegal. If I have an **`using` directive** in a ***class scope***, this is also illegal:
```
...
class D : private B {
public:
    void g() {
        using namespace std;
        cout << "From D: \n";
    }
    void h() {
        using std::cout;
        cout << "From E: \n";
    }
    using B::f;
    using std::cout;            // illegal
    using namespace std;        // illegal
};
...
```

### Methods from Parent won't Do Function Overloading because Name Hiding
In this example, again, we have the base class `B` and the derived class `D`. Now `D` is publicly derived from `B`, so it will inherit `B`'s public method `f()` as its own public method. The `B` has a method `f()`, with the integer parameter `a`. `D` has a method `f()` with no parameter. In the `main()` function, I create a `D` object `d`, and then called `d.f(8)`. You would expect the method `f()` of `B`, which takes an integer parameter will be invoked, but it will not. This code will not compile:
```
class B {
public:
    void f(int a);
};

class D : public B {
public:
    void f();
};

int main() {
    D d;
    d.f(8);             // Compiler error
}
```

This turns out `B`'s method `f()` is shadowed by `D`'s own method `f()`. This is called ***name hiding***. To overcome name hiding, we could use **`using` declaration for class member**. Now this code will compile:
```
class B {
public:
    void f(int a);
};

class D : public B {
public:
    using B::f;         // `using` declaration for class member
    void f();
};

int main() {
    D d;
    d.f(8);             // OK
}
```


### Anonymous Namespace
***Anonymous namespace*** is a **namespace that doesn't have a name**. As a result, everything inside of the namespace can be accessed within the same file. So in the `main()` function, I can call the `h()` function as if **`using` directive** is included implicitly for the ***anonymous namespace***. However, you cannot call the `h()` function from another file:
```
namespace {
    void h() {
        std::cout << "h()\n";
    }
}

int main() {
    h();
}
```

So this is effectively similar to having defined a ***global static function*** of `h()`:
```
// equivalent to this
static void h() {
    std::cout << "h()\n";
}
```

However, there is an additional benefit of using ***anonymous namespace***. For example, if I have a global version of function `g()` and also have another function `g()` inside the ***anonymous namespace***. When `h()` function called `g()`, it will call the **local version of `g()`**, **NOT** the global version:
```
void g();

namespace {
    void g();

    void h() {
        std::cout << "h()\n";
        g();                    // call local version `g()`
    }
}

int main() {
    h();
}
```




# Section 26 - Koening Lookup - Argument Dependent Lookup (ADL)
We're going to talk about ***Koening Lookup***.

Say we have a namespace `A`. Inside `A`, we've define a `struct` called `X` and a function `g()`. In the `main()` method, I have defined a `A::X` object `x1` and then invoke `g(x1)`:
```
namespace A {
    struct X {};
    void g(X x) {
        std::cout << " calling A::g() \n";
    }
}

int main() {
    A::X x1;
    A::g(x1);
}
```
This code will prints out `" calling A::g()"`, this is no question about that. However, if I remove `A::` and only calling `g(x1)`, what will happen?
```
namespace A {
    struct X {};
    void g(X x) {
        std::cout << " calling A::g() \n";
    }
}

int main() {
    A::X x1;
    g(x1);          // still prints out " calling A::g()"
}
```
You may expect that the compiler will error out and saying *"cannot find the function called `g()`"*, because the function `g()` is only defined inside the namespace `A`. However, this code will not only compiler, it will still printing out `" calling A::g()"`. It turns out when the compiler see the function `g()`, it will not only search the function `g()` in the current scope and the global scope. It will also search the function in the scope where its parameter type is defined. In this case, the type of its parameter is `X`, and `X` is defined in namespace `A`, so the compiler will search the `g()` function in the namespace `A`. That is how this `g()` function (in namespace `A`) is found. This phenomenon is called ***Koening Lookup*** or ***Argument Dependent Lookup* (ADL)**.

With ***Koening lookup***, we have increased the function name search scope, so if I have another global function also called `g()`, this code `g(x1)` will not compile, because there are two `g()` functions visible inside the `main()` function:
```
namespace A {
    struct X {};
    void g(X x) {
        std::cout << " calling A::g() \n";
    }
}

void g(A::X x) {
    std::cout << " calling global g() \n";
}

int main() {
    A::X x1;
    g(x1);          // Compiler error: call of overloaded 'g(A::X&)' is ambiguous
}
```



