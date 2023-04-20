# Section 1 - Initializer List & Uniform Initialization
In this section, I'm going to give you an introduction to ***C++11* standard**. What are the new features? Are they really useful?

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



## Uniform Initialization
***C++03*** allows me to initialize an **\**aggregate class*** or `struct` with ***curly braces enclosed list***. This is called ***aggregate initialization***.
```
class Dog {
public:
    int age;
    std::string name;
};

Dog d1 = { 5, "Henry" };    // Aggregate Initialization
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

