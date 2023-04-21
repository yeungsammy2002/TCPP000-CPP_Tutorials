# Section 11 - Static Initialization Fiasco
We'll talk about static initialization fiasco problem, which is a subtle problem that can crash your program. We'll talk about how this crash can be introduced and what is the common solution to this problem.

Let's look at our example. We have two classes `Cat` and `Dog`.

Let's first look at `Dog.h`. `Dog` has a private data member `_name`, which is the name of the `Dog`. And it also has a `bark()` method, and a constructor that takes a ***string* `name`** as parameter:
```
/* Dog.h */

#include <string>

class Dog {
    std::string _name;
public:
    void bark();
    Dog(char* name);
}
```
In `Dog.cpp`, the `bark()` method of the `Dog` prints out `"Dog rules! My name is {{ _name }}"`. And the constructor of the `Dog` prints out `"Constructing Dog {{name}}"`, and then assign the `name` to the private data member `_name`:
```
/* Dog.cpp */

#include "Dog.h"

void Dog::bark() {
    std::cout << "Dog rules! My name is " << _name << std::endl;
}

Dog::Dog(char* name) {
    std::cout << "Constructing Dog " << name << std::endl;
    _name = name;
}
```
The `Cat` class is very similar to the `Dog` class. But instead of having `bark()` method, it has a `meow()` method. Here is the `Cat.h`:
```
/* Cat.h */

#include <string>

class Cat {
    std::string _name;
public:
    void meow();
    Cat(char* name);
}
```
In `Cat.cpp`, the print out message is slightly different, it prints out `"Cat rules! My name is {{ _name }}"`:
```
/* Cat.cpp */

#include "Cat.h"
#include "Dog.h"
#include <iostream>

void Cat::meow() {
    std::cout << "Cat rules! My name is " << _name << std::endl;
}

Cat::Cat(char* name) {
    std::cout << "Constructing Cat " << name << std::endl;
    _name = name;
}
```
In the `main()` function, I created a global variable `d`, which is a `Dog` object with the name of `"Gunner"`. And inside the `main()` function, I call `d.bark()`:
```
Dog d("Gunner");

int main() {
    d.bark();
}
```
Let's run the program. It prints out the following messages on the console:
```
Constructing Dog Gunner
Dog rules! My name is Gunner
```

Now let's say I want to create another global variable in `Dog.cpp`. It's a `Cat` object `c` with the name of `"Smokey"`. And inside the `Dog`'s constructor. I'm going to call `c.meow()`:
```
/* Dog.cpp */

#include "Dog.h"

Cat c("Smokey");

void Dog::bark() {
    std::cout << "Dog rules! My name is " << _name << std::endl;
}

Dog::Dog(char* name) {
    std::cout << "Constructing Dog " << name << std::endl;
    _name = name;
    c.meow();
}
```
Now we have two global variables. One is the `Cat` object `c`, and another one is `Dog` object `d`. And **since they are in different source file, the order of the initialization is undefined**. In other words, whether the `d` will be constructed before `c`, or `c` constructed before `d` is undefined. This will cause a big problem, because when we construct `d`, this constructor will call `c.meow()`. So at this point, if `c` is already constructed, then we're fine. If `c` is not constructed, then the program will crash. So in other words, if `d` is constructed after `c`, then the program will live. If `c` is constructed after `d`, then the program will die. So our program has 50 by 50 chance of dying.

Now let's run the program, this actually runs okay, which means the `Cat` is constructed before the `Dog`:
```
Constructing Cat Smokey
Constructing Dog Gunner
Cat rules! My name is Smokey
Dog rules! My name is Gunner
```
So if we comment this `c.meow()` out:
```
...
Dog::Dog(char* name) {
    std::cout << "Constructing Dog " << name << std::endl;
    _name = name;
    // c.meow();
}
```
And in the `Cat`'s constructor (`Cat.cpp`), we call `d.bark()`, and we also need to add `extern Dog d;` because `d` is defined in another source file:
```
/* Cat.cpp */

#include "Cat.h"
#include "Dog.h"
#include <iostream>

extern Dog d;

void Cat::meow() {
    std::cout << "Cat rules! My name is " << _name << std::endl;
}

Cat::Cat(char* name) {
    std::cout << "Constructing Cat " << name << std::endl;
    _name = name;
    d.bark();
}
```
Now this program should crash because we know that the `Cat` is constructed before the `Dog`. So when we call `d.bark()`, the `Dog` object `d` is not constructed yet. So this program should crash. Let's run it, as you can see, the program crash.

To solve this problem, there are different ways to solve this problem, but the most commonly used is the ***singleton design pattern***.


### Singleton Design Pattern
I have created another class `Singleton` (`Singleton.h`), and then the `Singleton` has a `Dog`'s pointer `pd`, a `Cat`'s pointer `pc`, a destructor, a `getDog()` method, and a `getCat()` method:
```
/* Singleton.h */

class Dog;
class Cat;

class Singleton {
    static Dog* pd;
    static Cat* pc;

public:
    ~Singleton();

    static Dog* getDog();
    static Cat* getCat();
}
```
In the `Singleton` source file `Singleton.cpp`, both `pd` and `pc` are initialized to `0`. The `getDog()` method will check if `pd` is `0`, if `pd` is `0`, it create a new `Dog` object with a name `"Gunner"`, and then return `pd`. And next time when the `getDog()` method called, it will not create a new `Dog`, it will return the same `Dog` object `pd`. This is called ***initialized upon first usage idiom***. The same thing for the `Cat`. The `Cat` will be initialized only when it needs to be used:
```
/* Singleton.cpp */

#include "Singleton.h"
#include "Dog.h"
#include "Cat.h"

Dog* Singleton::pd = 0;
Cat* Singleton::pc = 0;

Dog* Singleton::getDog() {
    if(!pd)
        pd = new Dog("Gunner");     // Initialize Upon First Usage Idiom
    return pd;
}

Cat* Singleton::getCat() {
    if(!pc)
        pc = new Cat("Smokey");
    return pc;
}

Singleton::~Singleton() {
    if(pd) delete pd;
    if(pc) delete pc;
    pd = 0;
    pc = 0;
}
```
Now I don't need any global variable of `Cat` or `Dog`. Whenever I need a `Dog`, I call the `getDog()` method. And when I need a `Cat`, I call the `getCat()` method. And there will only be one `Dog` of `"Gunner"` and one `Cat` of `"Smokey"` available.

So in the `main()` function, let's say I'll call `Singleton::getCat()->meow()`.
```
int main() {
    Singleton::getCat()->meow();
}
```
And in the `Cat`'s constructor (`Cat.cpp`), I'll call `Singleton::getDog()->bark()`:
```
/* Cat.cpp */

#include "Cat.h"
#include "Dog.h"
#include <iostream>

void Cat::meow() {
    std::cout << "Cat rules! My name is " << _name << std::endl;
}

Cat::Cat(char* name) {
    std::cout << "Constructing Cat " << name << std::endl;
    _name = name;
    Singleton::getDog()->bark();
}
```
And we don't need any global variable of `Cat` anymore. We should always call `getCat()` when we need a `Cat`:
```
/* Dog.cpp */

#include "Dog.h"

void Dog::bark() {
    std::cout << "Dog rules! My name is " << _name << std::endl;
}

Dog::Dog(char* name) {
    std::cout << "Constructing Dog " << name << std::endl;
    _name = name;
}
```
Now let's run the program. Here is the output on the console:
```
Constructing Cat Smokey
Constructing Dog Gunner
Dog rules! My name is Gunner
Cat rules! My name is Smokey
```
So the program finished without a crash. The program should not crash because both `Dog` and `Cat` will be created whenever they are needed. And note that the `Singleton` has a destructor if `pd` and `pc` are not `0`, they will be deleted, and then reassign them to zero.

So to make sure that both the `Dog` and `Cat` will be deleted, we only need to create a `Singleton` instance of `s`. When `s` goes out of scope, it will automatically delete either the `Dog` or the `Cat`, or both. So both `Dog` and `Cat` will be constructed and deleted only if they are used. If they are not used, nothing will happen. This is the beauty of ***singleton design pattern***.




# Section 12 - `struct` & `class`
We'll talk about the difference between `struct` and `class`.

Here we have a `struct` of `Person_t`, and it has a string `name`, and an unsigned integer `age`. And we also has a `class` of `Person`, and it also has a string `name_`, and an unsigned integer `age_`. Now what is the different between these two:
```
struct Person_t {
    std::string name;
    unsigned age;
}

class Person {
    std::string name_;
    unsigned age_;
}
```
The difference is for `struct`, its data member are ***public*** by default. For `class`, its data member are ***private*** by default. So this is the only difference between `struct` and `class` as far as the language is concerned.

But usually, we'll add another level of semantic difference between `struct` and `class`. 
- `struct` is used for **small, passive objects that carry public data, and have no or few basic member methods**. In other words, `struct` is used for data container.
- `class` is usually used for bigger and more complicated active objects that carry private data and it interface with outside world through public member methods. In other words, a `class` is used for more complex data structure.

But this is just a convention. There's nothing in the language that enforces you to follow this convention. For example, nothing can stop you from giving the `struct` a private data - integer `height`:
```
struct Person_t {
    std::string name;
    unsigned age;
private:
    int height;
}
```
And nothing can stop you from giving a `class` public data - integer `height`:
```
class Person {
    std::string name_;
    unsigned age_;
public:
    int height;
}
```

However, this is a good to follow the convention, and you'd better follow it unless you have some special reason. Another convention is the **`class` data member are usually named with a trailing underscore** and the `struct` data member are named as the same way you name a regular variable.


### Getter & Setter
Now in the `main()` function, I create a `Person_t` object called `PT`. And I can access `PT`'s data member directly `PT.age`. For example, I want to print it out, and then I create a `Person` object `P` and I also want to print out `P.age_`:
```
int main() {
    Person_t PT;
    std::cout << PT.age;

    Person P;
    std::cout << P.age_;
}
```
This will not compiler because `age_` is a private data of the `Person`.

If I really want to access the private data of a class, I have to provide some public interface for that `.age()`, and all this method does is return the `age_`. Since this method only fetch the data of `age_` and doesn't change it, so we should make this method a `const` method. If later on, I do want to change the `age_`, then we need to provide another public method `set_age()` to change the `age_`:
```
class Person {
    std::string name_;
    unsigned age_;
public:
    unsigned age() const {          // getter / accessor
        return age_;
    }
    void set_age(unsigned a) {
        age_ = a;                   // setter / mutator
    }
};
```
This `age()` method usually called ***getter*** or ***accessor*** method, which is almost always a `const` method. And this `set_age()` method is usually called ***setter*** or ***mutator*** method.

And then in the `main()` function, I can call `P.age()`. And I can set the `age_` to a different value:
```
int main() {
    Person P;
    std::cout << P.age();
    P.set_age(4);
}
```
Some of you maybe start complaining *"If we have a private data member, and we defined **getter** and **setter** for this private data member. Then what's the point of making this data member private? We could just make it public and all of sudden, it saves us the additional effort of defining these member methods"*.

When we create object-oriented code, we want to separate the interface from the implementation. We want to encapsulate the complicated implementation and only talk to the outside world through interfaces. In this case, these `age()` and `set_age()` methods are interface. And these variables `name_` and `age_` are implementation. If you make the data member public, essentially you are exposing the internal implementation to the outside world, which usually bring a lot of headache in the long run.

For example, if you later on decided that `age_` is not a appropriate name for this variable, you want to change it with setter and getter methods, it is very easy to change the variable name. But if the data member is public and your client has been using it all over the place, then you have to change this variable `age_` in many different places.

Another example is if you want to add some rules to setting the `age_`, for example, a `Person`'s age should not be larger then `200`. Then with the ***setter method***, you can easily add the rule check at the beginning of this method. But if the variable `age_` is public, then there's no easy way to enforce that rule.

So this is the idea of having setter and getter methods instead of making the variable public.


### Avoiding Too Many Setter & Getter methods
On the other hand, having too many of setter and getter methods is also not a good design practice. If I have to provide setter and getter methods for many of my private data member, it means that my data needs to be used by somebody else. If that is the case, why the data belongs to me? why the data not belongs to somebody else? So having too many setting and getter methods indicates there's some problem with my design, maybe I should architect my design models in a different way. So in general, you should avoid too many of setter and getter methods in your code.


### Trailing Underscore Convention for Private `class` data members
As a side note, some of you might be curious why we weird convention of trailing underscore `name_`, `age_` for `class` data members. In the very old days, many people use `m_age` to denote a `class` data member. But then people start to think why using two characters to denote `class` data members if we could just use one. So some people think of using leading underscore `_age` for that purpose, and actually, the leading underscore is the offical naming convention in ***Python*** for private class data member. But the problem with C++ is the leading underscore is already used for other purpose, it is used for global variables, it is used for internal implementation related variables, particularly, the ***underscore followed by a capital letter*** and ***double underscore*** are widely used in the internal implementation. This is why people have invented the ***trailing underscore*** notation for the `class` data member. But again, a convention is a convention, there's nothing right or wrong here. It is most important for you to be consistent with the existing convention that is already used in the project that you are working on.


### Summary
1. Use `struct` for ***passive objects*** with ***public data***, use `class` for ***active objects*** with ***private data***.
2. Use ***setter***/***getter*** to access `class`'s data instead of making that data ***public***.
3. Avoid making ***setter*** / ***getter*** if possible.




# Section 13 - Resource Managing Class
We are going to talk about a pitfall that happens when you have a class that owns another object through its pointer.

Here is an example of this. We have a class `Person`, and this `Person` has a private data member, which is a ***pointer to a string* `pName_`**. And in this `Person`'s constructor, it create a new `std::string` object with this `name`. And in the `Person`'s destructor, it deletes that `pName_`. So this is what I mean by one class is only another class by pointer. In this case, the `Person` owns the ***string*** through its ***pointer* `pName_`**:
```
class Person {
public:
    Person(std::string name) {
        pName_ = new std::string(name);
    }
    ~Person() {
        delete pName_;
    }

    void printName() {
        std::cout << *pName_;
    }

private:
    std::string* pName_;
}

int main() {
    std::vector<Person> persons;
    persons.push_back(Person("George"));

    persons.back().printName();

    std::cout << "Goodbye" << std::endl;
}
```
Note that if the `Person` has the pointer of `pName_`, but it is not responsible of creating and destructing of the object of `pName_`. Then this `Person` is not owning that object of `pName_`. It merely has acquaintance with that object of `pName_`.

And this `Person` also have a `printName()` method, which prints out the name. And in the `main()` function, I create a vector of `Person` called `persons`. And then I create a `Person` who's name is `"George"`, and push it back into the `persons`, the vector. Since I'm using the `.push_back()` method, the `persons`'s `back()` method will return a reference to the last `Person` object in the vector, namely the `Person` - `"George"`. And then I invoke the `.printName()` method from that `Person` object.

If I run the program, it actually crash. If we look at the call stack, it crash at the point where the `.printName()` method is called. This code looks so innocent. I create a `Person`, push it into vector and then call its `.printName()` method. What could go wrong?

Let's analyze what has happened with this line of code. What happened over here is actually different for ***C++11* standard** than for ***older standard***. I will mainly talk about the ***older standard***. In the future, we'll cover the ***C++11***:
```
    persons.push_back(Person("George"));
```
With the ***older standard***, this simple line of code actually involves three operations. 
- Step 1. This `Person` - `"George"` is constructed
- Step 2. A copy of `"Georage"` is saved in the vector `persons`
- Step 3. This `Person` - `"George"` is destroyed

There are couple of things that we need to pay attention to. First of all, this `Person` - `"George"` and this `Person` in the vector are **NOT** the same `Person`. They are identical, but they are not the same, they are different objects. One `Person` is copy of the other. 

Secondly, when we make a copy of this `Person` - `"George"` and saved in the vector, we are making a ***shallow copy*** of this `Person` - `"George"`, because that's what the default copy constructor would do. So as a result of that, this `Person` - `"George"`'s pointer `pName_` and the `Person` in the vector's pointer `pName_` are pointing to the same object of string.

Lastly, this `Person` - `"George"` is a ***r-value***, which means it will be destroyed at the end of this statement. And when the `Person` - `"George"` is destroyed. It will call the destructor, and it will delete the object that pointing to by the `pName_` pointer. And as a direct result of that, the `Person` in the vector, who's pointer `pName_` will be pointing to a deleted object of string. So when we call the `Person` in the vector's `.printName()` method, it will access an object that is already deleted. This is why this program has crashed.

So what should we do with this kind of `class`? There are two solutions that you can use:
- ***Solution 1***: Define ***copy constructor*** and ***copy assignment operator*** for ***deep copying***.
- ***Solution 2***: Delete ***copy constructor*** and ***copy assignment operator***, define `clone()` method.


### Solution 1
The first solution is define ***copy constructor*** and ***copy assignment operator*** for ***deep copying***. So let's define a copy constructor, it will make a ***deep copy***. Since we need to access this private data member, so we need to provide an ***accessor*** to that data member. And we should do the same thing for ***copy assignment operator***, and this one should also do ***deep copying***:
```
class Person {
public:
    Person(std::string name) {
        pName_ = new std::string(name);
    }
    Person(const Person& rhs) {                     // Copy constructor
        pName_ = new std::string(*(rhs.pName()));
    }
    Person& operator=(const Person& rhs);           // Deep copying, Copy assignment operator
    ~Person() {
        delete pName_;
    }
    void printName() {
        std::cout << *pName_;
    }
    std::string* pName() const {
        return pName_;
    }

private:
    std::string* pName_;
};
...
```
Now if we run the program, it run through okay. And here is the output on the console:
```
GeorgeGoodbye
```


### Solution 2
The second solution for this is delete the ***copy constructor*** and the ***copy assignment operator***. And we can do that by making the ***copy constructor*** and the ***copy assignment operator*** **private methods**, so that nobody else can use them. And better than that, we can remove the definition of these two methods (*the function body*), so that even the `Person`'s children and `friend`s cannot use these two methods:
```
class Person {
public:
    Person(std::string name) {
        pName_ = new std::string(name);
    }
    void printName() {
        std::cout << *pName_;
    }
    std::string* pName() const {
        return pName_;
    }

private:
    std::string* pName_;
    Person(const Person& rhs);              // Copy constructor
    Person& operator=(const Person& rhs);   // Copy assignment operator
};

int main() {
    std::vector<Person> persons;
    persons.push_back(Person("George"));

    persons.back().printName();

    std::cout << "Goodbye" << std::endl;
}
```
So now we have two solutions, ***Solution 1*** and ***Solution 2***, which one should we prefer? Personally, I prefer the ***Solution 2*** - delete the copy constructor and the copy assignment operator. If you think about it, there's rarely the case where you have to define a copy constructor and a copy assignment operator. One common place where you want to define these two methods is when you're working with ***STL* containers**. ***STL* containers** requires their "containee" to be **copy constructible** and **copy assignable**, but we can easily work around this by saving an object's pointer in the containers instead of the object itself. And when we push back, we push back a pointer of the `Person`. And when we call the `.printName()` method, we use the pointer syntax. And of course, we need to remember to delete all the objects later on:
```
...
int main() {
    std::vector<Person*> persons;
    persons.push_back(new Person("George"));

    persons.back()->printName();

    std::cout << "Goodbye" << std::endl;
}
```
So this will work as expected.


### Solution 2 with Clone Method
However, even though we can live without the copy constructor and the copy assignment operator. Sometimes, we do need to make a copy of an object. How can we do that if we don't have these two methods? We can define a ***clone method***.
```
class Person {
public:
    Person(std::string name) {
        pName_ = new std::string(name);
    }
    void printName() {
        std::cout << *pName_;
    }
    std::string* pName() const {
        return pName_;
    }
    Person* clone() {                       // here
        return (new Person(*(pName_)));
    }

private:
    std::string* pName_;
    Person(const Person& rhs);
    Person& operator=(const Person& rhs);
};
```
So this `clone()` method will return an identical copy of `this` object itself. You can all the places where the copying has happened by grabbing the method name `clone` of all your code.

The problem with copy constructor is making the copying implicit. The copying can happen implicitly when an object is passed as a parameter to another function, or being return from another function. And implicitly copying is often time the place where the bug is introduced. So our ***Solution 2*** is delete copy constructor and copy assignment operator, and then define `clone()` method.

So in summary, anytime you have one object owning another object through its pointer like this `string* pName_;`, you need to implement one of these solutions. First one is define the ***copy constructor*** and the ***copy assignment operator*** for ***deep copying***. And second one is delete the ***copy constructor*** and the ***copy assignment operator***, then define `clone()` method.

