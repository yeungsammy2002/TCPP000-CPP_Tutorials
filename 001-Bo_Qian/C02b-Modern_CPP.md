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
    boVector(boVector& rhs) {           // Copy constructor
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
Since we are already using ***C++11*** and we have defined a ***move constructor*** for our object, so this kind of function `foo_by_value(boVector v)`, we shouldn't call it `foo_by_value` because when this function is taking a ***r-value*** as a parameter, it will actually call the ***move constructor*** to move the object. So we'll just call it `foo()` **\*here**. Suppose that the `reusable` will no longer be reused, so after this `foo(reusable)` function call, reusable will be destroyed. Even though `reusable` is a ***l-value***, we don't want to make a copy of it. And pass it to `foo()`, we want to reuse the object for the `foo()` function. What can we do? We can call `foo()`, and then call the standard library function `std::move()`. This will move the object of `reusable` to the `foo()` function with the ***move constructor***. But you need to be very careful that after you call the `std::move()` function with the `reusable`, the `reusable`'s member `reusable.arr_` is equal to `nullptr`. So you really shouldn't be using the object of `reusable` again after call the `std::move()` function on it. When the `reusable` is destroyed, it will call its destructor, which will delete the array. In this case, it is just deleting a `nullptr`.
```
class boVector {
    int size;
    double* arr_;
public:
    boVector(boVector& rhs) {
        size = rhs.size;
        arr_ = new double[size];
        for(int i = 0; i < size; i++) {
            arr_[i] = rhs.arr_[i];
        }
    }
    boVector(const boVector&& rhs) {
        size = rhs.size;
        arr_ = rhs.arr_;
        rhs-arr_ = nullptr;
    }
    ~boVector() {
        delete arr_;
    }
};

void foo(boVector v);                       // *here
void foo_by_ref(boVector& v);

boVector createBoVector();

int main() {
    boVector reusable = createBoVector();
    foo(reusable);                          // *here
    foo(std::move(reusable));
    // reusable is destroyed here
}
```

There is one last thing that I want to talk about in this example, suppose we have another function call `foo_by_ref(reusable)`, so now we have three function calls. This one `foo(reusable)` will call the ***copy constructor***. And this one `foo(std::move(reusable))` will call the ***move constructor***. And this funtcion `foo_by_ref(reusable)` will call no constructor, it won't call any constructor at all:
```
...
int main() {
    boVector reusable = createBoVector();

    foo_by_ref(reusable);               // Call no constructor
    foo(reusable);                      // Call copy constructor
    foo(std::move(reusable));           // Call move constructor
}
```
If you look at the efficiency of these three function calls, this one `foo_by_ref(reusable)` is the most efficient function call. And this one `foo(std::move(reusable))` is almost as efficient as the first one `foo_by_ref(reusable)`. The second function call is the most expensive function call.


A couple of things to note:
- **Note 1**: The most useful place for ***r-value reference*** is overloading ***copy consntructor*** and ***copy assignment operator***, to achieve ***move semantics***. You are already seen an example of overloading ***copy constructor*** with a ***move constructor***. Similarly, we can overload the ***copy assignment operator*** with a ***move assignment operator***.
    ```
    X& X::operator=(X const& rhs);
    X& X:operator=(X&& rhs);
    ```

- **Note 2**: Move semantics has already been implemented for **all *STL containers***, which means two things:
  - If you are using ***STL containers*** and you move to ***C++11***, your code might get faster without changing a single line of code. This is because all the unnecessary copy construction of ***STL containers*** will be automatically replaced with ***move construction***.

  - ***Passing-by-value*** can always be used for ***STL containers***. For example, we have a `foo()` function, which returns a vector by value. This is perfectly fine. And then the `hoo()` function pass over a parameter of string `s` by value, and this is okay no matter how big the string is. The only place you want to use pass by reference is when you want the function to return a certain value through its parameter like the case of the `goo` function:
    ```
    std::vector<int> foo() {
        ...
        return myvector;
    }

    void hoo(std::string s);

    void goo(std::vector<int>& arg);        // Pass by reference if you use `arg` to carry
                                            // data back from goo()
    ```


## Summary
The main purpose of move constructor and move assignment operator is to conveniently avoid costly and unnecessary ***deep copying***:
1. They are particularly powerful where **passing by reference** and **passing by value** are both needed for your object. If you know that your object will always be passed by reference, then this is not necessary to create the move semantics for your object.
   
2. They give you finer control of which part of your object to be moved. Basically you can do anything you want in the ***move constructor*** and the ***move assignment operator***.




# Section 4 - *r-value* Reference - Perfect Fowarding
We are going to talk about the second usage of ***r-value reference***, which is called ***perfect forwarding***.

Let's look at our example. We have a function `foo()`, which takes a `boVector` as an argument, and as we know that, `boVector` has both ***move constructor*** and ***copy constructor*** defined. We have a second function `relay()`, which is template function, it will take its argument and pass the argument over to the function `foo()`. This is called ***argument forwarding***. The `relay()` function will be invoked with a ***l-value*** or with a ***r-value***. As we talked about in previous section, when the `relay()` function is invoked with ***l-value***, then the `boVector`'s copy constructor will be invoked. When the `relay()` function is invoked with ***r-value***, then the ***move constructor*** of `boVector` will be invoked. That's the whole purpose of ***moving semantics***:
```
void foo(boVector arg);

template<typename T>
void relay(T arg) {
    foo(arg);
}

int main() {
    boVector reusable = createBoVector();
    relay(reusable);
    ...
    relay(createBoVector());
}
```
Now let's take a closer look at the parameter forwarding that happens inside the `relay()` function. In the ideal world, how should the `relay()` function pass over its argument to `foo()` function.

There are at least two requirements that need to be met:
1. First one is no costly and unnecessary copy construction of `boVector` is made. The costly copy construction of `boVector` should be made only when it's necessary.
   
2. Secondly, the argument that's being passed to function `relay()`, should have the same type of the argument that being forwarded to function `foo()`. Specifically, if a ***r-value*** is being passed over to `relay()` function, a ***r-value*** should be passed over to `foo()` function. If a ***l-value*** is passed over to `relay()` function, a ***l-value*** should be passed over to `foo()`. So only then, we can called it ***perfect forwarding*** of the parameter. In other words, ***r-value*** is forwarded as ***r-value***, and ***l-value*** is forwarded as ***l-value***.

Our Solution is actually quite simple, all we need to do is rewrite the `relay()` function like this. The `relay()` function will a parameter `T&&`, and it will call the standard library's `std::forward<T>()` function to the argument before passing the argument over to function `foo()`:
```
template<typename T>
void relay(T&& arg) {
    foo(std::forward<T>(arg));
}
```
If all you want is a solution, you can even stop here. You already know the syntax, you know how to achieve perfect forwarding parameter. However, I do encourage you to finish the entire section because the rest of this section will not only dive into the detail how the perfect forwarding is achieved. It also will give you an introduction of some other important feature of ***C++11***.


## Reference Collapsing Rules (***C++11***)
Let's look at ***reference collapsing rules***. ***C++11*** defined a ***reference collapsing rules*** for ***type deduction***:


### Case 1. `T& &` ==> `T&`
If I have a **`T` reference** `T&` to a **reference** `&`, it will be deduced to **`T` reference** `T&`. Note that you as a programmer cannot write code like this `T& &`. But the compiler can generate a code like this `T& &`, and then deduce its type to this `T&`


### Case 2. `T& &&` ==> `T&`
In the same way, a **`T` reference** `T&` to **double reference** `&&` will be deduced `T&`.


### Case 3. `T&& &` ==> `T&`
`T` **double reference** `T&&` to **reference** `&` is a **`T` reference**.


### Case 4. `T&& &&` ==> `T&&`
**`T` double reference** to **double reference** `&&` is a **`T` double reference** `T&&`


The rule is actually not hard to remember, it seems that the **single ref** `&` is a ***"infectious"***, whenever there's appearance of **single ref** `&` (***Case 1, 2 & 3***), then the result of the deduction is a **single ref** `T&`. The result is a **double reference** `T&&` (***Case 4***) only when there is **double refs** in their type `T&& &&`, there is **no single ref**.



Let's look at a standard library structure call the `remove_reference`. `remove_reference` does exactly what the name said, it remove reference. So if I create a `remove_reference<int&>`, then the resulted type is an integer. So this line of code `remove_reference<int&>::type i` is exactly the same as this code `int i`:
```
template<class T>
struct remove_reference;            // It removes reference on type `T`

// T is int&
remove_reference<int&>::type i;     // int i;
```

If I create the `remove_reference<int>`, because there is no reference to remove, so the result is also `int i`:
```
// T is int
remove_reference<int>::type i;      // int i;
```


Now let's go back the solution of function `relay()` that I give you. The function `relay()` takes a parameter of `T&&`:
```
template<typename T>
void relay(T&& arg) {
    ...
}
```