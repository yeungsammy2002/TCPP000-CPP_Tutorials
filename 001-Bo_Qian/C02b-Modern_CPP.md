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

Now the question is, what is the usefulness of this kind of function overloading? In our simple example, these kind of overloading (`printInt(int& i)` & `printInt(int&& i)`) is not very useful, because the parameter `i` is a very small integer. But if the parameter is a resource managing objects, then this kind of overloading becomes very useful.



## Move Constructor
Let's look at a real realistic example. Here we have a class called `boVector`, which is managing a big array of `double`s. It has a ***copy constructor***, which basically creates a new array, and then copy each and every item from the right-hand side `boVector` to itself. So this is very costly deep copy. It also has a ***destructor***, which basically delete the array. The `foo()` function is a function that takes a `boVector` as a parameter. And then the `createBoVector()` function creates a new `boVector`. In the `main()` function, first I create a new `boVector` that is called `reusable`, because it is intended to be used, and then we use the function `createBoVector()`. And then we call the function `foo(reusable)`. This will invoke the costly ***copy constructor*** to make an exactly copy of `reusable`, and then pass it to the function `foo()`. This is an expensive operation, but I'm okay with it. Because the `reusable` will be reused later on. I don't want function `foo()` to mess up the content of the `reusable`. Later on, I call the `foo()` function again, but this time, I'm calling with `boVector` object that created by `createBoVector()` function directly. And again, this one will call the costly copy constructor to create a new `boVector` and pass it to `foo()`:
```
class boVector {
    int size;
    double* arr_;                       // A big array
public:
    boVector(const boVector& rhs) {    // Copy constructor
        size = rhs.size;
        arr_ = new double[size];
        for(int i = 0; i < size; i++) {
            arr_[i] = rhs.arr_[i];
        }
    }
    ~boVector() {
        delete arr_;
    }
};

void foo(boVector v);

boVector createBoVector();              // Creates a boVector

int main() {
    boVector reusable = createBoVector();
    foo(reusable);

    foo(createBoVector());
}
```
Will I be okay with it now? Definitely not. Because the `createBoVector()` will return a ***r-value***, which is a temporary. That will be destroyed momentarily. So what's the point of making a copy of the temporary and passed to `foo()`? Why don't we just use this already created object and then pass it to `foo()` directly? That is exactly what we are going to do.

We'll have another constructor, and this one is taking a ***r-value reference*** as parameter. This is called ***move constructor***. The ***move constructor*** doesn't create a new array. Instead, it just take the right-hand side array `rhs.arr_` and use it. However, we need to clear the right-hand side array to `nullptr`. This is what we mean by ***move constructor***. We're moving the right-hand size `boVector`'s array into `this` `boVector`. It is very important to remember to set the right-hand side `boVector`'s array to `nullptr`. Otherwise, when the right-hand side's `boVector` is destroyed, its destructor will delete this array. Since we need to change the right-hand side `boVector`, we don't want the parameter to be a constant:
```
class boVector {
    int size;
    double* arr_;                       // A big array
public:
    boVector(boVector& rhs) {    // Copy constructor
        size = rhs.size;
        arr_ = new double[size];
        for(int i = 0; i < size; i++) {
            arr_[i] = rhs.arr_[i];
        }
    }
    boVector(const boVector&& rhs) {    // Move constructor
        size = rhs.size;
        arr_ = rhs.arr_;
        rhs-arr_ = nullptr;
    }
    ~boVector() {
        delete arr_;
    }
};

void foo(boVector v);

boVector createBoVector();              // Creates a boVector

int main() {
    boVector reusable = createBoVector();
    foo(reusable);

    foo(createBoVector());
}
```
So the move constructor is making an inexpensive ***shallow copy*** and the copy constructor is making an expensive ***deep copy***. When the first `foo()` function call, see its parameter as a ***l-value***. It will call the expensive copy constructor. And the second function call see its parameter as a ***r-value***, and it will call the inexpensive ***move constructor***. So this is the kind of flexibility and the efficiency you can achieve with the ***move constructor***.

If we don't have ***r-value reference*** and ***move constructor***, but we want to achieve the same kind of efficiency, we would have to have different version of the `foo()` function. We will have one version that called the `foo_by_value()`, and then we will have a different version called `foo_by_ref()` that's taking `boVector` reference as parameter. First, we call `foo_by_value()`, and then call `foo_by_ref()`. So you will have a lot of function by value and function by reference in your code, which will make it very messy:
```
class boVector {
    int size;
    double* arr_;                       // A big array
public:
    boVector(boVector& rhs) {    // Copy constructor
        size = rhs.size;
        arr_ = new double[size];
        for(int i = 0; i < size; i++) {
            arr_[i] = rhs.arr_[i];
        }
    }
    boVector(const boVector&& rhs) {    // Move constructor
        size = rhs.size;
        arr_ = rhs.arr_;
        rhs-arr_ = nullptr;
    }
    ~boVector() {
        delete arr_;
    }
};

void foo_by_value(boVector v);
void foo_by_ref(boVector& v);

boVector createBoVector();              // Creates a boVector

int main() {
    boVector reusable = createBoVector();

    foo_by_value(reusable);
    foo_by_ref(createBoVector());
}
```
Since we are already using ***C++11*** and we have defined a ***move constructor*** for our object, so this kind of function `foo_by_value(boVector v)`, we shouldn't call it `foo_by_value` because when this function is taking a ***r-value*** as a parameter, it will actually call the ***move constructor*** to move the object. So we'll just 

# 3 - 9:29

