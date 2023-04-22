# Section 1 - Initializer List & Uniform Initialization
In this section, I'm going to give you an introduction to ***C++11* standard**. What are the new features? Are they really useful?



## C++11 Feature 1 - Initializer List
The first one is ***initializer list***. In ***C++03***, you can initialize an array like this:
```
int arr[4] = { 3, 2, 4, 5 };
```
This is called ***initializer list***. This is nice. However, if you have a ***vector*** and the only way to initialize the vector is pushing every value into the vector, which is not convenient:
```
std::vector<int> v;
v.push_back(3);
v.push_back(2);
v.push_back(4);
v.push_back(5);
```

So ***C++11*** extends the support of initializer list to many other classes. So you can initializer a vector the same way you initialize an array:
```
std::vector<int> v = { 3, 4, 1, 9 };
```
What this does is it calling an ***initializer list constructor***. Note that ***all the relevant STL containers*** has been updated to accept `initializer_list` like ***map*** or ***list***.

Beyond that, you can define your own `std::initializer_list` constructor for your own class. In this example, I have a class `boVector`, and `boVector` has `std::initializer_list` constructor defined over **\*here**. This constructor takes up a parameter of `std::initializer_list`, and in the constructor, I go through each item in the `std::initializer_list` and push the item into my own private data member `m_vec`:
```
#include <initializer_list>
class boVector {
    std::vector<int> m_vec;
public:
    boVector(const std::initializer_list<int>& v) {                                         // *here
        for(std::initializer_list<int>::iterator itr = v.begin(); itr != v.end(); ++itr)
            m_vec.push_back(*itr);
    }
};
```
Having that, I can initialize a `boVector`, the same way I initialize an array:
```
boVector v = { 0, 2, 3, 4 };
```
I can also initialize `boVector` like this, which is effectively the same:
```
boVector v{ 0, 2, 3, 4 };
```



## C++11 Feature 2 - Uniform Initialization
***C++03*** allows me to initialize an **\**aggregate class*** or `struct` with ***curly braces enclosed list***. This is called ***aggregate initialization***:
```
class Dog {
public:
    int age;
    std::string name;
};

Dog d1 = { 5, "Henry" };    // Aggregate Initialization
```

***C++11*** extended the scope of curly brace initialization to any classes, so as long as the `Dog` has a constructor that takes an integer and a string, it can also be initialized with the same format of ***curly braces enclosed list***:
```
class Dog {
public:
    Dog(int age, std::string name) { ... };
};

Dog d1 = { 5, "Henry" }
```
So the regular constructor initialization, the aggregate initialization and the initializer list initialization, they all take on the same format of ***curly brace enclosed list***. That is why they are called ***uniform initialization***.

However, in the eyes of the compiler, the three kinds of initialization are not seen as equal. They have different priorities. The ***uniform initialization*** will take **`std::initializer_list` constructor** as its **first choice**. And a ***regular constructor*** as the **second choice**. And an ***aggregate initializer*** as the **last choice**.

#### Uniform Initialization Search Order:
1. `std::initializer_list` cosntructor
2. Regular constructor that takes the appropriate parameters
3. Aggregate initializer

So when the compile see a `Dog` is initialized with curly brace `3`. The first thing we will do is search the class `Dog` for `std::initializer_list` constructor like **\*this**. If that is found, it will take `3` as a single item array and pass it over to the `std::initializer_list`. If that is not found, it will search for a constructor **\*here** that takes a single integer as a parameter. If this is not found either, it will try to take `Dog` class as an ***aggregate class*** and call the aggregate initializer for the `age` **\*here2**:
```
Dog d1{ 3 };

class Dog {
public:
    int age;                                            // *here2

    Dog(int a) {                                        // *here
        age = a;
    }

    Dog(const std::initializer_list<int>& vec) {        // *this
        age = *(vec.begin());
    }
};
```



---
**\*Aggregate class** is a class that has no user-declared constructors, no private or protected non-static data members, no base classes, and no virtual functions.

In other words, an aggregate class is a class that only contains public data members and does not have any non-default constructors, destructors, or other special member functions. An aggregate class is often used to represent simple data structures, such as a point in a 2D coordinate system, or a rectangle.

Here is an example of an aggregate class in C++:
```
struct Point {
    int x;
    int y;
};
```
In this example, the Point class has two public data members, `x` and `y`, but no user-defined constructors, destructors, or other special member functions. Therefore, it is an aggregate class.

One important thing to note is that C++11 introduced the concept of an "aggregate initialization", which allows you to initialize an aggregate class using a brace-enclosed initializer list. For example:
```
Point p  { 1, 2 };
```
In this example, the `Point` object `p` is initialized using an initializer list, which is allowed because `Point` is an aggregate class.

---



## C++11 Feature 3 - `auto` Type
Let's say we have a vector `vec`. In ***C++03***, this is what we typically do to traverse a vector. I don't know about you, but for me, it is always wrecks me to have to type a long type name like this `std::vector<int>::iterator ...`. 
```
std::vector<int> vec = { 2, 3, 4, 5 };

for(std::vector<int>::iterator it = vec.begin(); it != vec.end(); ++it)
    m_vec.push_back(*it);
```
So I am thrilled to see C++ introduced `auto` type. With `auto` type, it can automatically infer the type for it from its r-value, the right-hand side value, in this case, `vec.begin()`. This `auto it ...` will save me a lot of typing, and the less typing I do, the less typo I will make:
```
for (auto it = vec.begin(); it != vec.end(); ++it)
    m_vec.push_back(*it);
```
Here are some other example. Initialize `a` with `6`, `a` becomes an ***integer***. Initialize `b` with `9.6`, `b` becomes a ***double***. Initialize `c` with `a` because `a` is an ***integer***, `c` become an ***integer*** too.
```
auto a = 6;           // a is a integer
auto b = 9.6;         // b is a double
auto c = a;           // c is an integer
```
The side effect of this is **IDE** become more important because you want to hover your mouse over a variable and see what type it is. Otherwise, you have to fumble around to find the type because all you see is `auto`.



## C++11 Feature 4 - For Each
Again here's how we traverse a vector in ***C++03***:
```
for(std::vector<int>::iterator itr = v.begin(); itr != v.end(); ++itr)
    std::cout << (*itr);
```

In ***C++11***, I can do the same thing with much simplified coding like this. What this mean is for each item of `v`, assign it to `i`, and do something with `i`. This kind of coding can work on any class of `v` that has `.begin()` and `.end()` methods:
```
for(int i : v)           // works on any class that has .begin() and .end()
    std::cout << i;     // read only access
```
If you remember, I can change `int` into `auto`:
```
for(auto i : v)          // good for long type name such as shared_ptr
    std::cout << i;
```

If I want to change to the value of `v`, all I need to do is adding a **reference sign `&`** in front of `i`, and in this case, I'm incrementing each member of `v` by one, so I'm changing the value of `v`:
```
for(auto& i : v)
    i++;
```



## C++11 Feature 5 - `nullptr`
In ***C++03***, ***null pointer*** is represented with `NULL`, which is defined with integer `0`. However, this could be a problem, suppose I have a function `foo()` with ***integer***, and another function `foo()` with ***`char` pointer*** `char*`. When I call `foo(NULL)`, which function am I calling? The `foo()` with integer? Or the `foo()` with `char*`?
```
void foo(int i) {
    std::cout << "foo_init" << std::endl;
}

void foo(char* pc) {
    std::cout << "foo_char*" << std::endl;
}

int main() {            // don't know which function should be called
    foo(NULL);
}
```

***C++11*** introduce a new keyword `nullptr`, which is dedicated to represent the ***null pointer***. So which I call `foo(nullptr)`, it is very clearing I'm calling the `foo()` with `char*`:
```
void foo(int i) {
    std::cout << "foo_init" << std::endl;
}

void foo(char* pc) {
    std::cout << "foo_char*" << std::endl;
}

int main() {            // call second function
    foo(nullptr);
}
```



## C++11 Feature 6 - `enum class`
In ***C++03***, ***enumerators*** are basically ***integers***. Here I enum `apple`, which include green apple `green_a` and red apple `red_a`. And enum `orange`, which include big orange `big_o` and small organe `small_o`. Then I initialize `apple` enumerator `a` with `green_a`, and initialize `orange` enumerator `o` with `big_o`. As a result, I can compare `apple` to `orange`:
```
enum apple { green_a, red_a };
enum orange { big_o, small_o };
apple a = green_a;
organe o = big_o;

if(a == 0)
    std::cout << "green apple and big orange are the same\n";
else
    std::cout << "green apple and big orange are not the same\n";
```
This code will indeed prints out `"green apple and big orange are the same"`.

***C++11*** introduced `enum class`. The `apple` and `orange` are not just enumerators, they are classes. So when I define `apple` object `a` and `organe` object `o`, I have to add the class name with scope operator - `apple::` and `orange::` in front of the enumerators. Now if I compare the `a` and `o`, it will give me an compile error, because I haven't define the equality operator for class `apple` and class `orange`:
```
enum class apple { green, red };
enum class orange { big, small };
apple a = apple::green;
orange o = orange::big;

if(a == o)
    std::cout << "green apple and big orange are the same\n";
else
    std::cout << "green apple and big orange are not the same\n";
```
As you can see, the `enum class` and the `nullptr` have made the C++ more strong type and more safe to use.



## C++11 Feature 7 - `static_asert()`
We are all familiar with assertion, at any ***runtime*** of the program. We can assert a certain condition is `true`. In this example, I assert that `myPointer` is not `NULL`:
```
assert(myPointer != NULL);
```
***C++11*** provide a `static_assert`, which allows you to make a assertion during the ***compile time***. In this example, I statically assert that the size of integer is equal to `4`, which means the following code will not work if the integer size is not `4`:
```
static_assert(sizeof(int) == 4);
```



## C++11 Feature 8 - Delegating Constructor
It is very common for the constructors to share the same code. So sometimes it is desirable to have this kind of code as below. I have defined a first constructor. And then define a second constructor, which reuse the code of the first constructor, and then do something else. However, this code can only work in ***Java***. It won't work in ***C++***. At least it won't work as you expected. It typically will create two `Dog`s instead of one:
```
class Dog {
public:
    Dog() { ... }
    Dog(int a) {
        Dog();
        doOtherThings(a);
    }
    ...
};
```

So in ***C++***, we often come ends up having code like below. It define a `init()` method, and the `init()` method will be invoked at different constructors:
```
class Dog {
    void init() { ... }
public:
    Dog() {
        init();
    }
    Dog(int a) {
        init();
        doOtherThings();
    }
    ...
}
```
The downside of this kind of implementation is, first of all, it's cumbersome. Comparing to the first code snippet, I have to define additional method. And this additional method will be duplicated in each constructor. Secondly, the `init()` method is a regular method, it could be invoked by any other methods, which means the `init()` method needs to take care of the additional complexity of being invoked at the different life state of the object, not just the construction stage of the object.

***C++11*** provides a new way to share the code of the constructors. The first constructor can be called at the initialization list section of the second constructor. This allows the first constructor to be invoked before the second constructor starts. The limitation here is the first constructor cannot be invoked in the middle of the second constructor, or in the end of the second cosntructor:
```
class Dog {
public:
    Dog() { ... }
    Dog(int a) : Dog() {
        doOtherThings();
    }
}
```
***C++11*** also allows ***in-class data memeber initialization***. So if the `Dog` has a data member `age`, I can initialize over **\*here** with 9. As a result, `age` will be initialized at every constructor:
```
class Dog {
    int age = 9;            // *here
public:
    Dog() { ... }
    Dog(int a) : Dog() {
        doOtherThings();
    }
}
```