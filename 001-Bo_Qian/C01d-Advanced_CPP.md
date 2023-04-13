# Section 11 - Static Initialization Fiasco

We'll talk about static initialization fiasco problem, which is a subtle problem that can crash your program. We'll talk about how this crash can be introduced and what is the common solution to this problem.

Let's look at our example. We have two classes `Cat` and `Dog`.

Let's first look at `Dog.h`. `Dog` has a private data member `_name`, which is the name of the `Dog`. And it also has a `bark()` method, and a constructor that takes a ***string* `name`** as parameter:
```
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