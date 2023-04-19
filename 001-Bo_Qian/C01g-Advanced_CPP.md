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
***Software engineering*** is largely about ***code reuse***.
```
class BaseDog {

};

class BullDog : public BaseDog {

};

class ShepherdDog : public BaseDog {

};
```
