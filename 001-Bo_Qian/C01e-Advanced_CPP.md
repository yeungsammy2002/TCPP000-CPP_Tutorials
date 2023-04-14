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

There are four casting operators in C++ - ***static cast***, ***dynamic cast***, ***const cast***, and ***reintepret cast***.


### Static Cast
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


### Dynamic Cast
Now let's look at ***dynamic cast***. First I create a `YellowDog`, and then cast the pointer to a `Dog`'s pointer. Now this is a ***static cast***, even though I'm not using the `static_cast` operator. Then I cast the `Dog`'s pointer `pd` back to `YellowDog`'s pointer `py`:
```
Dog* pd = new YellowDog();                      // static case
YellowDog py = dynamic_cast<YellowDog*>(pd);
```
***Dynamic cast*** can only work on ***pointers*** or ***references***. It cannot work on an ***object***. And they convert from one type to a related type, and typically it will be used in ***down cast***, which means it casts an object from its ***base class*** to a ***derived class***. In this case, from a `Dog` to a `YellowDog`.

# 16 - 3:41


### Const Cast


### Reintepret Cast


