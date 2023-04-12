# Section 07 - Exceptions in Destructors
We'll talk about preventing exception from leaving destructors. Exceptions provide a way for your program to react to exceptional circumstances, such as a runtime error. They do that by transferring the control to a special portion of the code called ***exception handler***.

***Destructor*** like any other part of the code, it may throw an ***exception***. However, you need to be very careful that the exceptions thrown by destructors are contained inside of the destructors, they should not be thrown out of the ***destructor***.

To understand why, let's look at some simple example. We have a class `Dog`, and `Dog` has a constructor and a destructor. Each one of them will print out some message, so we know they are executed. `Dog` also have a method called `bark()`. In the `main()` function, we created two `Dog` objects - `dog1`, `"Henry"` and `dog2`, `"Bob"`. And before `"Henry"` and `"Bob"` can bark a little, exception of `20` is thrown out. The exception of `20` will be caught by the `catch`-block:
```
class Dog {
public:
    std::string m_name;
    Dog(std::string name) {
        m_name = name;
        std::cout << name << " is born." << std::endl;
    }
    ~Dog() {
        std::cout << m_name << " is destroyed.\n" << std::endl;
    }
    void bark() { ... }
    ...
};

int main() {
    try {
        Dog dog1("Henry");
        Dog dog2("Bob");
        throw 20;
        dog1.bark();
        dog2.bark();
    } catch(int e) {
        std::cout << e << " is caught" << std::endl;
    }
}
```
Here is the output on the console:
```
Henry is born.
Bob is born.
Bob is destroyed.
Henry is destroyed.
20 is caught
```
`"Henry is born."` first because `"Henry"` is created first, then `"Bob is born"` and `"Bob is destroyed"`. Why `"Bob"` is destroyed first? Because `"Henry"` and `"Bob"` are stored on ***stack***. ***Stack*** is ***Last-In-First-Out***, because `"Bob"` is born last, it will be destroyed first. And then `"Henry"` is destroyed, and `20` is caught.

The main point that I want to make with this example is before the exception of `20` is caught by the `catch`-block, the ***stack*** will unwind and all the local variables inside the `try`-block needs to be destroyed. So both `"Bob"` and `"Henry"` will be destroyed before `20` is caught.

Having that in mind that we remove the `throw` statement. Now there will be no exceptions in the `main()` function. However, I will add a `throw` exception in the `Dog` destructor.
```
class Dog {
public:
    std::string m_name;
    Dog(std::string name) {
        m_name = name;
        std::cout << name << " is born." << std::endl;
    }
    ~Dog() {
        std::cout << m_name << " is destroyed.\n" << std::endl;
        throw 20;
    }
    void bark() { ... }
    ...
};

int main() {
    try {
        Dog dog1("Henry");
        Dog dog2("Bob");
        dog1.bark();
        dog2.bark();
    } catch(int e) {
        std::cout << e << " is caught" << std::endl;
    }
}
```
Let's see what happens when an exception is thrown out of a destructor. If we run this program, it will crash.

When the execution reach the end of `try`-block, the stack will unwind and all the local variables needs to be destroyed. So `"Bob"` will be destroyed first. When `"Bob"`'s destructor execute, it throw exception of `20`. As we have learned previous experiment, it throw the exception `20` is caught by the `catch`-block. `"Henry"` also need to be destroyed, and `"Henry"` destructor also execute and also throw an exception. So as a result, will have two exceptions pending and at the same time, one from `"Bob"`, another one from `"Henry"`. C++ doesn't like having more the one exception pending at the same time, so it will just crash. This is why we should not throw an exception out of a destructor.

Our solution number one is the destructor swallows whatever the exception that it generates. In this example, I put all the ***exception prone code*** inside a `try`-block, and catch the exception that I expected. You can also add another `catch`-statement with ***ellipsis* `...`**. This will prevent any other unknown exception from leaking out of the destructor:
```
    ...
    ~Dog() {
        try {
            // Enclose all the exception prone code here
        } catch (MYEXCEPTION e) {
            // Catch exception
        } catch (...) {
            // ...
        }
    }
    ...
```

# 7 - 4:58