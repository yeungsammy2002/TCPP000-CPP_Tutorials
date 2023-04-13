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