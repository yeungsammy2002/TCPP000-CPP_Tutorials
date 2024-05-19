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

When the execution reach the end of `try`-block, the stack will unwind and all the local variables needs to be destroyed. So `"Bob"` will be destroyed first. When `"Bob"`'s destructor execute, it throw exception of `20`. As we have learned previous experiment, it throw the exception `20` is caught by the `catch`-block. `"Henry"` also need to be destroyed, and `"Henry"` destructor also execute and also throw an exception. So as a result, will have two exceptions pending and at the same time, one from `"Bob"`, another one from `"Henry"`. C++ doesn't like having more than one exception pending at the same time, so it will just crash. This is why we should not throw an exception out of a destructor.


### Solution 1 - Destructor Swallow the Exception
Our solution number one is the destructor swallows whatever the exception that it generates. In this example, I put all the ***exception prone code*** inside a `try`-block, and catch the exception that I expected. You can also add another `catch`-statement with ***ellipsis* `...`**. This will prevent any other unknown exception from leaking out of the destructor. However, you have to be very careful about this kind of `catch`-statement because it's not very useful. It doesn't give you any error message. And secondly, sometimes it's even dangerous because this kind of statement will mask of any kind of exception:
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
The downside of this solution is since the exception is swallow by the destructor, the `Dog`'s client will not get the exception, so they will not know what has happened and do appropriate thing to handle that.


### Solution 2 - Move the Exception-Prone Code to a Different Function
This leads us to our second solution, which is move the exception prone code to a different function, not in the ***destructor***. So the destructor will contain code that is either exception free or having very little chance of throwing exception. For example, in our `Dog` example, I'll remove the exception thrown code in the destructor. And add another method called `prepareToDestr()`. And in this method, I'll do a bunch of different things and it may thrown exceptions. Now the destructor is exception free. And in the `main()` function, before `dog1` and `dog2` get destroyed, they need to call the `.prepareToDestr()` method:
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
    void prepareToDestr() {
        ...
        throw 20;
    }
    void bark() { ... }
};

int main() {
    try {
        Dog dog1("Henry");
        Dog dog2("Bob");
        dog1.bark();
        dog2.bark();
        dog1.prepareToDestr();
        dog2.prepareToDestr();
    } catch(int e) {
        std::cout << e << std::endl;
    }
}
```
Now first of all, this program will no longer crash. And secondly, the `Dog`'s client will get the exception and do a proper thing to handle the exception. The downside of this solution is one additional API for `Dog`'s client to call.

Which solution should we use? ***Solution 1 - destructor swallow the exception*** or ***Solution 2 - move the exception-prone code to a different function***? The answer depends on who is the better person to handle the exception. If it is a `Dog`, you should use ***Solution 1***. If it is `Dog`'s client, you should use ***Solution 2***.




# Section 8 - Virtual Method in Constructor or Destructor
I'm going to show you a **pitfall** in calling ***virtual method*** in ***constructor*** or ***destructor***.

Let's look at our example. We have a class `Dog`. And `Dog`'s constructor prints out `"Dog born."`. And `Dog`'s `bark()` method prints out `"I am just a dog"`. A `Dog` also has a `seeCat()` method, when a `Dog` see a cat, it barks. `YellowDog` is derived from a `Dog`. And the `YellowDog`'s constructor prints out `"Yellow dog born."`, and the `YellowDog`'s `bark()` prints out `"I am a yellow dog"`. In the `main()` function, I create a `YellowDog` object called `d`, and then call `d.seeCat()`:
```
class Dog {
public:
    Dog() {
        std::cout << "Dog born." << std::endl;
    }
    void bark() {
        std::cout << "I am just a dog" << std::endl;
    }
    void seeCat() {
        bark();
    }
};

class YellowDog : public Dog {
public:
    YellowDog() {
        std::cout << "Yellow dog born." << std::endl;
    }
    void bark() {
        std::cout << "I am a yellow dog" << std::endl;
    }
};

int main() {
    YellowDog d;
    d.seeCat();
}
```
Let's run the program. And here is the output on the console:
```
Dog born.
Yellow dog born.
I am just a dog
```
So when I create a `YellowDog` object `d`, it first call the `Dog`'s constructor. And then prints out `"Dog born."`. Then it call `YellowDog`'s constructor and prints out `"Yellow dog born."`. And then it calls `d.seeCat()`, and `d.seeCat()` calls the `bark()` method. Under the `.bark()` method, which prints out `"I am just a dog"`, even though this is a `YellowDog` object.

To help the `YellowDog` to be honest, we have to make this `Dog`'s `bark()` method `virtual`. So the `YellowDog`'s `bark()` method automatically becomes a `virtual` method. But in practice, it is a good idea to always put a `virtual` keyword in front of the method to make it explicit that this is a `virtual` method:
```
class Dog {
public:
    Dog() {
        std::cout << "Dog born." << std::endl;
    }
    virtual void bark() {
        std::cout << "I am just a dog" << std::endl;
    }
    void seeCat() {
        bark();
    }
};

class YellowDog : public Dog {
public:
    YellowDog() {
        std::cout << "Yellow dog born." << std::endl;
    }
    virtual void bark() {
        std::cout << "I am a yellow dog" << std::endl;
    }
};
```
Now let's run the program, and here is the output on the console:
```
Dog born.
Yellow dog born.
I am a yellow dog
```
So when the `.seeCat()` method invoke the `bark()` method, it realize that `this` object is a `YellowDog`, so it invoke the `YellowDog`'s method. This is the power of ***dynamic binding***. As long as the `d` is a `YellowDog` object, it will always call a `YellowDog`'s virtual method.



### Dynamic Binding May Not Work as Expected
However, there are some scenarios where this idea of dynamic binding is not working. Let's say the `bark()` method is invoked inside the `Dog`'s constructor to see what happen:
```
class Dog {
public:
    Dog() {
        std::cout << "Dog born." << std::endl;
        bark();                                         // here
    }
    virtual void bark() {
        std::cout << "I am just a dog" << std::endl;
    }
    void seeCat() {
        bark();
    }
};

class YellowDog : public Dog {
public:
    YellowDog() {
        std::cout << "Yellow dog born." << std::endl;
    }
    virtual void bark() {
        std::cout << "I am a yellow dog" << std::endl;
    }
};

int main() {
    YellowDog d;
    d.seeCat();
}
```
Here is the output on the console:
```
Dog born.
I am just a dog
Yellow dog born.
I am a yellow dog
```
This `bark()` method prints out `I am just a dog`, even though the `bark()` method is a virtual method. And we know that the object being created is a `YellowDog`, it is still calling `Dog`'s `bark()` method.

When `YellowDog` object `d` is created, before it calling the `YellowDog`'s constructor. It first call the `Dog`'s constructor. So at this point, the constructor of the `YellowDog` is not executed yet. In other words, the `YellowDog` is not constructed yet. We all know it is dangerous to call the member method of an object that is not constructed yet unless that method is a `static` method. So the compiler is doing the second best thing, which is calling the `Dog`'s own `bark()` method. So this `bark()` method behaves like a ***non virtual method*** when it is invoked inside a constructor.

The conclusion we are getting from this example is **we should avoid calling virtual method in a constructor**. A constructor should do as little as possible to put the object in valid state and that's it. Calling any fancy method inside the constructor is not recommended.

And at the same idea for ***destructor***. If we have a destructor on `Dog` that also call the `bark()` method, then we run the program:
```
class Dog {
public:
    Dog() {
        std::cout << "Dog born." << std::endl;
        bark();
    }
    virtual void bark() {
        std::cout << "I am just a dog" << std::endl;
    }
    void seeCat() {
        bark();
    }
    ~Dog() {
        bark();                                         // here
    }
};

class YellowDog : public Dog {
public:
    YellowDog() {
        std::cout << "Yellow dog born." << std::endl;
    }
    virtual void bark() {
        std::cout << "I am a yellow dog" << std::endl;
    }
};

int main() {
    YellowDog d;
    d.seeCat();
}
```
As you can see on the console, when the destructor of the `Dog` calling the `bark()` method, it also call the `Dog`'s `bark()` method (the last `I am just a dog`), because when the `d` gets destroyed, it first called the destructor of the `YellowDog`, and then call the destructor of the `Dog`. So at this point the `YellowDog` is already destroyed. And it is not a good idea to call the member method of something is already destroyed:
```
Dog born.
I am just a dog
Yellow dog born.
I am a yellow dog
I am just a dog
```




# Section 09 - Assignment to Self in Assignment Operator
One of the C++ critical features is ***operator overloading***. It can exploit people's intuition and reduce their learning curve for new libraries. ***Assignment operator*** `operator=` is one of the operators that's frequently being overloaded. In this section, we are going to talk about one pitfall in implementing ***assignment operator***, which is ***self-assignment***.

Here is an example of ***self-assignment***:
```
Dog dd;
dd = dd;                // look silly
```
You may ask will people really do stupid thing like this? No, they were not. But they may very well do things like this:
```
dogs[i] = dogs[j];      // looks less silly
```
This is a ***self-assignment*** when `i` and `j` are equal. That is why we need to handle ***self-assignment*** appropriately in our ***assignment operator***.


### Self Assignment Problem in `class` that Contains Pointer Member
Now let's look at another example. We have a class `Dog`, and `Dog` wears `Collar`. So this `Dog` has a data member `pCollar`, which is a pointer to a `Collar` class. And then the `Dog`'s ***assignment operator* `operator=`** will copy everything from the **right-hand side `Dog` reference `rhs`** to itself. In our case, the main thing to copy over is the `pCollar`. The natural implementation of that is I'll first delete my own `pCollar`, and then I copy construct the new `pCollar` from the right hand side of its `pCollar` - `*rhs.pCollar`:
```
class Collar;
class Dog {
    Collar* pCollar;
    Dog& operator=(const Dog& rhs) {            // if `rhs` is equals to `this`,
        delete pCollar;                         // it'll delete `rhs.pCollar` as well
        pCollar = new Collar(*rhs.pCollar);     // create `Collar` with deleted `pCollar` object
        return *this;
    }
};
```
However, there is a problem with this implementation. The problem arises when `this` `Dog` and right hand side `Dog` - `rhs` are the same `Dog` object. In other words, this is a ***self-assginment***. If that is the case, when I delete the `pCollar`, I'm also deleting the `pCollar` of the right hand side `Dog` - `rhs`. Then when I copy construct the `Collar` from the right hand side's `pCollar` - `*rhs.pCollar`, I'm accessing an object that is deleted, and the result could be disastrous.


### Solution 1 - Checking If there is Self-Assignment 
One simple solution is we can do a check. If `this` equals to the **right hand side's `Dog` reference `rhs`**, then we simply return `*this`:
```
class Collar;
class Dog {
    Collar* pCollar;
    Dog& operator(const Dog& rhs) {
        if(this == &rhs)                    // check if `rhs` is equals to `this`
            return *this;                   // if true, stop here and return this object

        delete pCollar;
        pCollar = new Collar(*rhs.pCollar);
        return *this;
    }
};
```
Now we have much better situation, the deleting and the copy constructing only happens when `this` `Dog` object and the **right hand side `Dog` object - `rhs`** are not the same `Dog` object.

However, there is still a problem with code. What happens if the copy constructor **\*here** of the right hand side's `Dog`s `pCollar` throws an ***exception***. In that case, the `Dog` has deleted its own `pCollar` but it failed to create a new `pCollar`. So the `Dog` ends up holding a pointer that's pointing to an invalid object. This is a big problem if the `Dog`'s client later on wants to use the `Dog` more. And even nobody is using the `Dog` anymore, when the `Dog` is destructed, the `Dog`'s destructor may want to try to delete the `pCollar` again, and the result is undefined:
```
class Collar;
class Dog {
    Collar* pCollar;
    Dog& operator=(const Dog& rhs) {
        if(this == &rhs)
            return *this;

        delete pCollar;
        pCollar = new Collar(*rhs.pCollar);     // if *here throw an exception,
        return *this;                           // `pCollar` will point to an invalid object
    }
};
```

So it seems what we really want to do is delete `pCollar` only after the new `pCollar` is created successfully. To achieve that purpose, I need to create a copy of original `pCollar`, and then I create a new `pCollar` if that is successful and I'll delete the original `pCollar`:
```
class Collar;
class Dog {
    Collar* pCollar;
    Dog& operator(const Dog& rhs) {
        if(this == &rhs)
            return *this;

        Collar* pOrigCollar = pCollar;          // copy original `pCollar` address, not memory
        pCollar = new Collar(*rhs.pCollar);     // if succeed, pCollar will points to another address
        delete pOrigCollar;                     // delete memory that original `pCollar` address points to
        return *this;
    }
};
```
Now we are safe. Even if the `new` operator throws an exception, the `Dog` still holding a pointer that points to a valid `Collar`. So this gives you a small demo of writing exception safe code.


### Solution 2 - Delegation
Now let's look at our ***Solution 2***. ***Solution 2*** bascially delegates the assignment operation to the `Collar` class, so I simply copy `*rhs.pCollar` to `*pCollar`. It will either do a **memeber by member copying** of the `Collar` class, or invoke the `Collar` class's ***overloaded assignment operator* `operator=`**:
```
class Dog {
    Collar* pCollar;
    Dog& operator=(const Dog& rhs) {
        *pCollar = *rhs.pCollar;            // member by member copying of collars or
                                            // call collar's `operator=`
        return *this;
    }
};
```
One thing to note is I don't necessarily need to check if `this` `Dog` object and the **right hand side's `Dog` object - `rhs`** are the same `Dog` object. If they are the same, I will make a copy of itself anyway. This may incur some runtime cost. But since it doesn't cause any serious trouble, I don't want to worry about the runtime cost now until later may profile that tells me that this is important.


Besides, the `if`-statement checking in ***Solution 1*** is not for free. It will also incur some runtime cost. So removing the `if`-statement will more or less compensate the cost of the ***self-assignment***.




# Section 10 - Resource Acquistion Is Initialization - RAII
We're going to talk about a special coding technique called ***Resource Acquistion Is Initialization* - *RAII***. What it basically means is using object to manage resources. Resources could be memory, hardware device, network handle...etc.


### Locking & Unlocking Mutex using RAII Technique
Let's look at an example. Here I have a ***mutex*** `mu`. And in `functionA()`, I lock the ***mutex***, and then do a bunch of things, and then unlock ***mutex***:
```
Mutex_t mu = MUTEX_INITIALIZER;

void functionA() {
    Mutex_lock(&mu);
    ...Doing a bunch of things...
    Mutex_unlock(&mu);  // Will this line always be executed?
}
```
This is pretty regular looking code, but there is a problem with this code. The problem is the statement of unlocking ***mutex*** may not be executed, because `...Doing a bunch of things...` could return to `functionA()` prematurely. Even if it doesn't return, it may throw an ***exception***. And in either case, the ***mutex*** will be locked forever. So neither way to guarantee that the ***mutex*** will be unlocked once is no longer needed.

Let's look at our solution. The solution is to use the ***Resource Acquisition Is Initialization* (RAII) technique**. Here I have a class `Lock`. The class `Lock` has a `private` data member, which is a ***pointer*** to ***mutex***. And in the constructor of the `Lock`, the ***mutex*** will be locked. And in the destructor of the `Lock`, ***mutex*** will be unlocked. Now in `functionA()`, whenever I want to lock the ***mutex***, I construct a `Lock`, and then do whatever things that I want to do. And by the end of `functionA()`, `mylock` will be destroyed from the ***stack***. And then the ***destructor*** of the `Lock` will be invoked. So ***mutex*** will be unlocked:
```
class Lock {
private:
    Mutext_t* m_pm;
public:
    explicit Lock(Mutex_t *pm) {
        Mutext_lock(pm);
        m_pm = pm;
    }
    ~Lock() {
        Mutex_unlock(m_pm);
    }
};

void functionA() {
    Lock mylock(&mu);
    ...Doing a bunch of things...
}   // The mutex will always be released when mylock is destroyed from stack
```
The conclusion we can take from this example is the only code that can guaranteed to be executed after exception is thrown at the destructor of the objects residing on ***stack***. So in this example, the destructor of `mylock` is guaranteed to be executed. So our resource management needs to be tied to the lifespan of a suitable objects in order to gain **automatic deallocation** and **reclamation**.


### `std::shared_ptr` is using RAII Technique
Another good example of ***RAII*** is ***shared pointer***. A ***shared pointer*** is a ***reference counting smart pointer***. It counts the number of pointer points to itself. And when that number reach `0`, the smart pointer will be destroyed. Now in `function_A()`, I created a new `Dog` and assign the new `Dog`'s pointer to a shared pointer `pd`. Once `pd` goes out of scope, or in other words, once there's no more pointer points to `pd`, the new `Dog` will be destroyed:
```
int function_A() {
    std::tr1::shared_ptr<Dog> pd(new Dog());
    ...
}   // The `Dog` is destructed when `pd` goes out of scope (no more pointer points to `pd)
```
Note that `std::tr1::shared_ptr` is equivalent to `std::stared_ptr` in ***C++11*** standard. `tr1` stands for ***Technical Report 1***.


### Avoiding Memory Leak when Initializing Shared Pointer `std::shared_ptr`
Let's look at another example. I have a class `Dog`, a class `Trick` and a function `train()`, which train a `Dog` with some kind of dog trick. I have a function `getTrick()` which returns a `Trick` object. Now, in the `main()` function, I invoke the `train()` function with the parameter `Dog` is created from a new `Dog`. And the parameter `dogTrick` is get from the `getTrick()` function:
```
class Dog;
class Trick;
void train(std::tr1::shared_ptr<Dog> pd, Trick dogTrick);
Trick getTrick();

int main() {
    train(std::tr1::shared_ptr<Dog> pd(new Dog()), getTrick());
}
```
Now, do you see any problem with this kind of code? Let's examine on what happens in the `train()` function parameter passing, more specifically, this part of the code `std::tr1::shared_ptr<Dog> pd(new Dog()), getTrick()`.

There are three things happening, one is creating a new `Dog`. Second thing is invoking the `getTrick()` function. Lastly, construct a ***shared pointer*** and assign the `Dog` to the ***shared pointer***:
1. `new Dog();`
2. `getTrick();`
3. construct `std::tr1::shared_ptr<Dog>`.

In C++, the order of the three operations are **NOT fixed**. It is completely determined by the compiler. So if the compiler decide to execute the three operations in this order, then there is a problem. The `getTrick()` function might throw an ***exception***. If that happen, when we have created a new `Dog` and the new `Dog` has not being assigned to a ***shared pointer*** yet, the memory of the new `Dog` will be leaked.

The conclusion we can take from this example is don't combine the operation of storing objects in a ***shared pointer*** with any other statement. In other words, **put the storing objects in shared pointer statement to a *standalone statement* \*here**. So in this example, we'll extract this part of the code `std::tr1::shared_ptr<Dog> pd(new Dog())` in `trick()` call and do something like **\*this**:
```
class Dog;
class Trick;
void train(std::tr1::shared_ptr<Dog> pd, Trick dogTrick);
Trick getTrick();

int main() {
    std::tr1::shared_ptr<Dog> pd(new Dog());        // *here
    train(pd, getTrick());                          // *this
}
```
Now our code is thread safe, because the new `Dog` once get created, it will be always assigned to a ***shared pointer***, and it is not impossible to have memory leak.


### Avoiding Resource Management Objects being Copied by Disallowing Compiler Generated Methods
What happens when the resource management object is copied? In this example, I create `L1` from a ***mutex*** `mu`, and then copy construct `L2` from `L1`. **Usually, the *mutex* is mutually exclusive, so it cannot be called shared by multiple clients**:
```
Lock L1(&mu);
Lock L2(L1);        // bad practice, `L1` should be shared
```
One solution for this is prohibit copying completely. We can do that by **disallow the *copy constructor* and the *copy assignment operator* from being used by our client**. And to see how to do that, please read my another section that is ***disallow compiler generated methods*** and we're not going to talk about that in this section.


### Using Reference Count Mechanism if Resource Management Objects Can Be Shared
Sometimes the resource can be shared by multiple clients. In those kind of scenario, we need to always make sure **all the resources will be released appropriately once all the clients are done with it**. So we need to implement some kind of ***reference count mechanism*** to keep track of the number of clients who are using the resource. We can do that with the ***shared pointer***.

In previous example, we have seen that shared pointer can take one parameter, which is a newly created object that's storing on ***heap***:
```
...
int main() {
    std::tr1::shared_ptr<Dog> pd(new Dog());
    train(getTrick(pd), getTrick());
}
```
The ***shared pointer (`std::shared_ptr`) constructor*** can take a ***second parameter***, which is a ***deleter***. A ***deleter*** is a function that will be invoked when the shared pointer is destroyed. The ***operator delete***, the default value for the ***deleter*** is the ***operator delete***:
```
template<class Other, class D> shared_ptr(Other* ptr, D deleter);
```
So in this example, once the shared pointer `pd` is destroyed, the default value of deleter will be performed upon the `new Dog()`:
```
    std::tr1::shared_ptr<Dog> pd(new Dog());
```
A good thing about the shared pointer is the ***deleter*** can be customized to any function you like. Here we're going to take advantage of that.

Now let's look at the new implementation of the `Lock` class. Now it has private data member `pMutex`, which is a ***shared pointer*** to a ***mutex***. The constructor of the `Lock` initialized the `pMutex` with the ***pointer*** of the ***mutex*** as the first parameter, and then `Mutex_unlock` function at the second parameter, the ***deleter***. In the body of the constructor, it will pre-lock the mutex. So what this means is when `Lock` object is created, it will lock the mutex. When there is no more pointer points to `pMutex`, the mutex will be unlocked:
```
class Lock {
private:
    std::tr1::shared_ptr<Mutex_t> pMutex;
public:
    explicit Lock(Mutex_t* pm) : pMutex(pm, Mutex_unlock) {
        Mutex_lock(pm);
        // The second parameter of shared_ptr constructor is "deleter" function
    }
};
```

Let's look at the example again. I created `Lock` object `L1` from mutex, and then copy construct `L2` from `L1`. By this time, we have two pointers point to `pMutex`, so the ***reference count*** is ***2***. By the time both `L1` and `L2` goes out of scope, the number of pointer points to `pMutex` will become `0`, that means `pMutex`'s ***deleter*** will be invoked. And then the mutex will be unlocked:
```
Lock L1(&mu);
Lock L2(L1);
```
So that completes our ***Solution 2***, which guarantees the unlocking of mutex once there is no client is using the mutex anymore.

