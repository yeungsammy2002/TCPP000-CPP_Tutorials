# Section 14 - Virtual Constructor - `clone()` Method
Last time we have talked about using `clone()` method to replace copy constructor and copy assignment operator. So that object copying can only happen explicitly. Now we'll talk about another advantage of using `clone()` method. 

Let's look at an example. We have a class `Dog`, and the `YellowDog`, which is derived from `Dog`. In the `main()` function, we have a `YellowDog` object called `d`, and then we call the function `foo()` with the pointer to the `YellowDog` object `d`. The function `foo()` takes a `Dog`'s pointer as parameter. And inside the function, we create a `Dog` object `c`, which is copy constructed from `d`:
```
class Dog {
};

class YellowDog : public Dog {
};

void foo(Dog* d) {
    Dog* c = new Dog(*d);
    // ...
    // play with `Dog` object pointer `c`
}

int main() {
    YellowDog d;
    foo(&d);
}
```
Do you see any problem with this code? We know that `d` is a `YellowDog`. So what the function `foo()` get really is a `YellowDog`. But when we copy construct `c`, we are copy constructing a `Dog`. So as a result of that, `d` is a `YellowDog`, and `c` is a `Dog`. So `c` is not really is an identical copy of `d`. This is not good, because a copy is a copy. If it is identical to its original, how can you call it a copy?

To solve this problem, we can use, again, a `clone()` method. We define a public virtual `clone()` method in `Dog` class, and this method will return a `Dog` object, which is copy constructed from itself. And then the `YellowDog` class also have a `clone()` method, but it will return a `YellowDog` object instead of `Dog`. And in the `foo()` function, instead of calling the copy constructor directly, we will the `clone()` method. And now, `c` is indeed a `YellowDog` object. So in a sense, the `clone` method is like a vitrual constructor. It make sure the appropriate type of object will be constructed no matter what kind of type the original object has been casted into:
```
class Dog {
public:
    virtual Dog* clone() {              // here
        return (new Dog(*this));
    }
};

class YellowDog : public Dog {
    virtual YellowDog* clone() {        // here
        return (new YellowDog(*this));
    }
};

void foo(Dog* d) {
    Dog* c = d->clone();                // `c` is a YellowDog
    //...
    // play with `YellowDog` object pointer `c`
}

int main() {
    YellowDog d;
    foo(&d);
}
```
One thing to note is this `Dog`'s `clone()` method returns a `Dog`'s pointer, and then this `YellowDog`'s `clone()` method returns a `YellowDog`'s pointer, even though one function is the overridden function of the other. This is an important feature called ***co-variant return type***. It allows an overridden virtual function, in this case, the `clone()` method to have a different return type, as long as its return type is derived from the base class method's return type. So this is the important C++ language feature that makes the virtual constructor possible.




# Section 16 - All Castings Considered Part 1
***Casting*** has always been confusing topic. Now exactly what is ***casting***? C++ is a ***strongly typed language***. So an object can be converted from one type to another. 

Generally speaking and there are two kinds of type conversion:
1. ***Implicit Type Conversion***
2. ***Explicit Type Conversion*** - is also called ***Casting*** 

There are four casting operators in C++:
1. ***static cast***
2. ***dynamic cast***
3. ***const cast***
4. ***reintepret cast***


- ### Static Cast
Let's start with ***static cast***. `static_cast` can convert an object from one type to another.

Here I initialize an integer `i` to `9`. And then I cast `i` from an integer to a `float`. `static_cast` can work on any type of type, as long as the type conversion is defined for those types:
```
int i = 9;
float f = static_cast<float>(i);                    // convert object from one type to another
```
Here I create a string `"Bob"`, and then cast the string to a `Dog`. The condition for this statement will work if the type conversion is defined for the conversion from string to `Dog`.
```
Dog d1 = static_cast<Dog>(std::string("Bob"));      // Type conversion needs to be defined
```
`static_cast` can also work on ***pointers***, or ***reference***. For pointers and reference, it can only cast an object from one type to a related type, that means you can cast an object from a base class down to a derived class. Or cast it from a derived class up to a base class. In this example, my `YellowDog` is derived from `Dog`, and after I create a `YellowDow`, I can cast it as a pointer to a `Dog`'s pointer:
```
Dog* pd = static_cast<Dog*>(new YelloDog());        // Convert pointer/reference from one type to a related type (down/up cast)
```


- ## Dynamic Cast
Now let's look at ***dynamic cast***. First I create a `YellowDog`, and then cast the pointer to a `Dog`'s pointer. Now this is a ***static cast***, even though I'm not using the `static_cast` operator. Then I cast the `Dog`'s pointer `pd` back to `YellowDog`'s pointer `py`:
```
Dog* pd = new YellowDog();                      // static case
YellowDog py = dynamic_cast<YellowDog*>(pd);
```
***Dynamic cast*** can only work on ***pointers*** or ***references***. It cannot work on an ***object***. And they convert from one type to a related type, and typically it will be used in ***down cast***, which means it casts an object from its ***base class*** to a ***derived class***. In this case, from a `Dog` to a `YellowDog`.

Another different from the static cast is the dynamic in additional to perform the type casting also perform a runtime time check. It will check if the types are compatible for the casting to succeed. In this case, it will check if `pd` is pointing to a `YellowDog`, and not just a `Dog`. If the casting succeed, the `py` will become `pd`. If it failed, `py` will equals to `0`, which is a ***null pointer***.

Finally, ***dynamic cast*** requires the two types to be polymorphic, which means they need to at least have one virtual method.


- ### Const Cast
***Const cast*** is used to cast away the ***constness***. In the example, I have the `const` `chat` pointer `str` points to `"Hello, world."`. And the `"Hello, world."` cannot be modified because it defines to be a constant. Then I can use `const_cast` to cast away the ***constness*** and then assign the result to `modifiable` `char` pointer. And now the `"Hello, world."` can be changed:
```
const char* str = "Hello, world.";
char* modifiable = const_cast<char*>(str);
```
Like the ***dynamic cast***, the ***const cast*** can only work on pointers or references. It cannot work on the object itself. So if I change this `char*` to `char`. It won't work that way:
```
char* modifiable = const_cast<char>(str);   // X
```
Unlike the ***dynamic cast***, the ***const cast*** can only work on same type. So both `str` and `modifiable` are both `char` pointers. Only difference is one is a constant and other one is not constant.


- ### Reintepret Cast
Our last cast operator is a ***reintepret cast***. In this example, I have a `p` pointer, which is point to some address `51110980`, and then I will use `reintepret_cast` to intepret the memory content in that address `51110980` into a dog. The `reinterpret_cast` can only work on ***pointers*** or ***references***. It cannot work on the object itself:
```
long p = 51110980;
Dog* dd = reinterpret_cast<Dog*>(p);        // re-intepret the bits of the object pointed to
                                            // The ultimate cast that can cast one pointer to any other type of pointer
```
If you compare to the previous three cast that works on pointers or references, the ***static cast*** is working on the ***related type***. And the ***dynamic cast*** also can work on ***related type***. The ***const cast*** can only work on ***same type***. And the ***reintepret cast*** can work on ***any type***. You can cast from one pointer to any other type of pointer with ***reintepret cast***. So this is the most powerful cast. 

And because it is so powerful, it also gives you a lot of room to make mistakes. That is why you always need to use extra caution when using the ***reintepret cast***.

Typcially, ***reintepret cast*** is used in ***low-level coding***, because it tends to generate code that's not portable.


## C-Style Casting
C++ also inherited that the old-fashioned casting style from ***C***, that's called ***C-style casting***. ***C-style casting*** has ***two forms***.

# 16 - 7:50
```
short a = 2000;
int i = (int)a;
int j = int(a);
```

