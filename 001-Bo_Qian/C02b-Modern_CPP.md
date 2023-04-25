# Seciton 3 - *r-value* Reference - Move Semantics (C++11 Feature)
We're going to talk about a new feature introduced in ***C++11*** called ***r-value reference***.

***r-value reference*** is mainly used for two things:
1. ***Moving Semantics***
2. ***Perfect Forwarding***

We'll talk about mainly talk about ***moving semantics***. In order to understand ***r-value reference***, you must have a good understanding of ***r-value*** and ***l-value***. If you don't, I highly recommend you study my another section called ***Understanding l-value & r-value (Section 20, C01f)***.

### r-value Reference
Say we have initialize an integer `a` to `5`. `a` is a ***l-value***. And then we have an integer reference `b`, which is initialized with `a`. `b` is a ***l-value reference***. Because it is a reference that is referencing to our ***l-value***, which is `a`. Before ***C++11*** we just call it ***reference***.
```
int a = 5;      // `a` is a l-value
int& b = a;     // `b` is a l-value reference (reference)
```
***r-value reference*** is represented with **double ampersand sign `&&`**. It is a refernce that is referencing a ***r-value***.
```
int&& c = 5;    // `c` is a r-value reference
```

So how can this thing be used? Let's look at an example. We have two `printInt()` functions. This first one is taking a ***l-value reference*** as a parameter, the second one is taking a ***r-value reference*** as a parameter. In the `main()` function, if we call `printInt(a)`, this will call the first function `printInt(int& i)`, because `a` is ***l-value***. If we call `printInt(6)`, this will call the second function `printInt(int&& i)` because `6` is a ***r-value***:
```
void printInt(int& i) {
    std::cout << "l-value reference: " << i << std::endl;
}

void printInt(int&& i) {
    std::cout << "r-value referenc: " << i << std::endl;
}

int main() {
    int a = 5;          // `a` is l-value
    printInt(a);        // Call `printInt(int& i)`
    printInt(6);        // Call `printInt(int&& i)`
}
```
So with the ***r-value reference***, we can overload the function based on the parameter type, whether the parameter is a ***l-value***, or ***r-value***. So this is the basic concept of ***r-value reference***.

One thing to note is if we have a function `printInt()` that takes `i` as an integer, this code won't work. Because `printInt(a)` wouldn't know which function to call (`printInt(int& i)` or `printInt(int i)`). And when the compiler see the second function call `printInt(6)`, it doesn't know which one to call either (`printInt(int&& i)` or `printInt(int i)`):
```
void printInt(int& i) { ... }
void printInt(int&& i) { ... }
void printInt(int i) { ... }

int main() {
    int a = 5;
    printInt(a);            // Compiler error
    printInt(6);            // Compiler error
}
```
So you can only overload the function with a ***r-value reference*** and a ***r-value reference***.

Now the question is, what is the usefulness of this kind of function overloading? In our simple example, these kind of overloading (`printInt(int& i)` & `printInt(int&& i)`) is not very useful, because the parameter `i` is a very small integer. But if the parameter is a resource managing objects

# 3 - 3:59
