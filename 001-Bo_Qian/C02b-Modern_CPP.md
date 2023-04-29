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


Now let's go back the solution of function `relay()` that I give you. The function `relay()` takes a parameter of `T&&`. What is `T&&`? I have kept telling you that ***r-value reference*** is specified with `{{ type }}&&`, but doesn't `{{ type }}&&` always indicate it's a r-value reference? The answer is no. The type of `T&&` depends on how `arg` is initialized. If `arg` is initialized with a ***r-value***, then `T&&` is ***r-value reference***. If it initialized with ***l-value***, then `T&&` is a ***l-value reference***. How does it happen?
```
template<typename T>
void relay(T&& arg) {
    ...
}
```

Say we invoke `relay()` function on an integer `9`, `9` is a ***r-value***. Then `T` will be replaced with `int&&`. Then `T&&` will become `int&& &&`. Applying the ***reference collapsing rule***, it becomes `int&&`. So `T&&` is equivalent to `int&&`, it's a ***r-value reference***:
```
// T&& variable is initialized with r-value => r-value reference
relay(9);       // => T = int&& => T&& = int&& && = int&&
```

If `relay()` function is invoked with ***l-value*** `x`, regardless of `x` is an ***integer*** or ***integer reference*** `int&`, as far as the type deduction is concerned, `T` will be replaced with `int&`. So `T&&` becomes `int& &&`, which is applying a ***reference collapsing rule*** `int&`. So `T&&` is ***integer reference*** `int&`, which is a ***l-value reference***:
```
// T&& variable is initialized with l-value => l-value reference
relay(x);       // => T = int& => T&& = int& && = int&
```
As you can see, by give the function `relay()` a `T&&` type of argument, we are give the function `relay()` an enormous power to take on any kind of argument. It can take on ***r-value***, ***l-value***, `const`, non-`const`...anything. This `T&&` is what ***\*Scott Meyers*** called a ***universal reference***, it's ***universal***.

Someone may say *"Hey Bo, now you are confusing me. At first you say `&&` means a r-value reference. And now you say `&&` means a universal reference. How do I know which one it is?"* It turns out that `T&&` is an ***universal reference*** only if the following two conditions are met:
1. First, `T` is a ***template type***. As we see in our `relay()` function, `T` is a ***template type***, **NOT** a regular type like integer or `double`.
   ```
   template<typename T>
    void relay(T&& arg) {
        ...
    }
   ```
   
2. Secondly, the ***type deduction***, in this case, the ***reference collapsing*** happens to `T`. As we see in this example also:
   ```
    // T&& variable is initialized with r-value => r-value reference
    relay(9);       // => T = int&& => T&& = int&& && = int&&

    // T&& variable is initialized with l-value => l-value reference
    relay(x);       // => T = int& => T&& = int& && = int& 
   ```
   So what this typically mean is `T` is a ***function template type***, not a ***class template type***.

So `T&&` is ***universal reference*** only when these two conditions are met. In any other case, when you see `T&&` is a ***r-value reference***.

---
***\*Scott (Douglas) Meyers*** (born April 9, 1959) is an American author and software consultant, specializing in the C++ computer programming language. He is known for his Effective C++ book series.

---

Now we are well equipped to tackle the ***perfect forwarding problem***. Here is the complete solution of ***perfect forwarding***. The `relay()` function will take a universal reference of `arg`, and then it perform the `std::forward<T>()` function on `arg`, and then pass it over to `foo()`:
```
template<typename T>
void relay(T&& arg) {
    foo(std::forward<T>(arg));
}
```
Here is the implementation of `forward()` function. What it essentially does is cast `arg` to the type of `T&&`. In other words, the `relay()` function will cast the `arg` back to the type of `T&&`, and then pass it over to `foo()`. So the `relay()` function and the `foo()` function will have the exact same type of argument of `arg`:
```
template<class T>
T&& forward(typename remove_reference<T>::type& arg) {
    return static_cast<T&&>(arg);
}
```
If function `relay()` gets a ***l-value***, function `foo()` will get a ***l-value***. If function `relay()` gets a ***r-value***, function `foo()` will get a ***r-value***. This is how ***perfect forwarding*** is achieved.



## `std::move<T>()` vs `std::forward<T>()`
Some people, especially beginners are confused by the function `std::move<T>()` and the function `std::forward<T>()`. They have similarity, they both perform static casting of their argument to a certain type. But the difference are the function `std::move<T>()` turns its argument into a ***r-value*** type. And function `std::forward<T>()` turns its argument to type of `T&&`, whatever the `T&&` is:
```
std::move<T>(arg);          // Turn `arg` into a r-value type
std::forward<T>(arg);       // Turn `arg` to type of `T&&`
```



## Summary
The main usage of ***r-value reference*** is in two places:
1. The first one is to achieve ***move semantics***. It's typically done by overloading the ***copy constructor***, and the ***copy assignment operator*** with different parameter type of ***r-value reference*** and ***l-value reference***.
2. The second usage is the argument for ***perfect forwarding***, or the parameter for perfect forwarding.




# Section 5 - User Defined Literals
We are going to talk about a new feature in ***C++11*** called ***user defined literals***.

What are ***literals***? ***Literals*** are ***constants***. ***C++*** has 4 kinds of ***literals***:
1. ***Integer literal***        - such as `45`
2. ***Floating point literal*** - such as `4.5`
3. ***Character literal***      - such as `'z'`
4. ***String literal***         - such as `"Dog"`

A ***literal*** can have a ***suffix*** to specify an exact ***type*** of the ***literal***. So `45` means the type is `int`, `45u` means `unsigned int`, `45l` means it is a `long`:
```
45;     // int literal
45u;    // unsigned int (suffix specifies type)
45l;    // long
```

And then the ***user defined literals*** are allow us to define our own ***suffix***, so that this whole thing `45u` can be treated as **new type of *literal***. Why do we want to do that? Let's look at an example.

Say we are using the older C++ standard and we want to define a variable `height`, which is of the type of `long double`. And we set `height` to `3.4`:
```
// C++99
long double height = 3.4;       // Metres? Centimeters? Inches?
```
What does this mean? ***3.4 metres***, ***3.4 cenimeters***, or ***3.4 inches***? We don't know. This is one of the thing that make our code right only. Only the writer of the code understand what the code is doing. If you are going to work on this project for a long time, somebody might be nice enough to tell you that they always use ***metres*** for height, and they always use ***kilo hertz* - *kHz*** for frequency... so on and so forth. Otherwise, if you are not the usual author of the code, then you have to fumble around thousands of line of code and to figure out what the units are, and wish yourself a good luck of that. So `long double height = 3.4;` is not a nice way of writing clean code, but we have been doing this for a long time. We just wished all the developers will stick to the convention, and always use the same units for the same kinds of values. But unfortunately, people does mess up with the units.

So ideally, we should have something like this. `height` is set to `3.4cm`. `ratio` set to `3.4cm` divided by `2.1mm`. Since `3.4cm` and `2.1mm` have different units, unit translationis required. So we translate `3.4cm` from `cm` to `mm`, and then divided by `2.1`. So this is how the code should be look like ideally:
```
height = 3.4cm;
ratio = 3.4cm / 2.1mm;          // ratio 3.4 * 10 / 2.1
```
However, ***C++*** doesn't support this kind of syntax. And even if ***C++*** does support this kind of syntax, some people may not like it, because the ***unit translation*** means ***runtime cost***. And they don't like runtime cost, they prefer to use their brain to do the job for the machine. And they have good reason for doing that, especially when the unit translation needs to happen many many times during the program execution.

To solve this problem, ***C++11*** provides the new feature of ***user-defined literals***. 

Let's look at some example. Here I have the ***user-defined literals*** to define the unit of ***centimeter* `_cm`**, ***meter* `_m`** and ***milimeter* `_mm`**. `operator""` means this is the declaration of ***user-defined literals***. `_cm` is the ***identifier***, the ***suffix***. It takes a `long double x` as a parameter, and then multiply `x` by `10`, and then return the result as a `long double`. For `_m`, which means ***meter***, `x` is multiplied by `1000`. For ***millimeter***, `x` is returned as is. So the final unit is ***millimeter***. Both ***centimeter*** and ***meter*** will be translated into ***millimeter***:
```
long double operator"" _cm(long double x) {
    return x * 10;
}
long double oeprator"" _m(long double x) {
    return x * 1000;
}
long double operator"" _mm(long double x) {
    return x;
}

int main() {
    long double height 3.4_cm;
    std::cout << height << std::endl;
}
```
In this `main()` function, I intitialized `height` to `3.4_cm`, which means it is 3.4 centimeter, and then I print out the `height`. Let's run the program:
```
34
```
It prints out `34`. So the ***centimeter value*** is translated into ***millimeter value***.

Now let's say I print out `height + 13.0_m`:
```
int main() {
    long double height = 3.4_cm;
    std::cout << (height + 13.0_m) << std::endl;
}
```
Let's see what happen:
```
13034
```
It prints out `13034`. Since the unit of the `13.0` is meter, so `13.0` needs to multiply by thousand and then plus the height. The result is `13034`.

Now let's print out ***130 millimeters*** divided by ***13 meters***:
```
int main() {
    std::cout << (130.0_mm / 13.0_m) << std::endl;
}
```
Let's run the program:
```
0.01
```
It prints out `0.01`. So this is how ***user defined literals*** work. By using ***user defined literals***, we provided convenience way to specify the ***units***.

However, we haven't solve all the problem yet, the unit translation still happens at ***runtime***. They still demand ***runtime cost***, so to minimize the runtime impact, all we need to do is **make this function *constant expression* `constexpr`**. Since all these functions take ***literals*** as ***input parameters***. We can safely assume that all these calcuations (i.e. `return x * 1000;`) can be performed at ***compile time***:
```
constexpr long double operator"" _cm(long double x) {
    return x * 10;
}
constexpr long double oeprator"" _m(long double x) {
    return x * 1000;
}
constexpr long double operator"" _mm(long double x) {
    return x;
}

int main() {
    long double height = 3.4_cm;
    std::cout << height << std::endl;
    std::cout << (height + 13.0_m) << std::endl;
    std::cout << (130.0_mm / 13.0_m) << std::endl;
}
```
So let's run the program again:
```
34
13034
0.01
```
The result is the same. However, now, all the unit translations are done by the ***compiler***. So they won't cost anything during the ***runtime***.


Let's look at the second example. Here I am defined the ***user-defined literals* `_bin`**, which converts a string representation of ***binary number*** into an ***integer***. Note that this ***user defined literal* `_bin`** takes two parameters. First one is a string `str`, a `const char` pointer `const char*`. And second one is `std::size_t`, `l`. Note that ***C++11*** only allow a handful of data type, which can be used as parameter of the ***user defined literals***. With the `char` pointer `char*` and `std::size_t`, I can calculate the value of the binary number, and then return it. In the `main()` function, I print out `"110"` binary number, `"1100110"` and lastly a pretty big number `"110100010001001110001"`:
```
int operator"" _bin(const char* str, std::size_t l) {
    int ret = 0;
    for(int i = 0; i < 1; i++) {
        ret = ret << 1;
        if(str[i] == '1')
            ret += 1;
    }
    return ret;
}

int main() {
    std::cout "110"_bin << std::endl;
    std::cout "1100110"_bin << std::endl;
    std::cout "110100010001001110001"_bin << std::endl;
}
```
Let's run the program:
```
6
102
1712753
```
So `"110"` is `6`. The second one `"1100110"` is `102`. And the last one `"110100010001001110001"` is a pretty big number `1712753`.

So the importance of ***user defined literals*** is that ***C++*** went a long way to make ***user defined types* (*classes*)** to behave same as ***built-in types***, and ***user defined literals*** push this effort even futher. It makes the ***user defined literals*** to behave the same as the ***built-in literals***. So this is more than just a ***syntactic sugar***.


## Restrictions of using User Defined Literals
The last thing I want to point out is the ***user defined literals*** have some ***restrictions*** on the ***input parameter types***. The ***input parameter*** can only be one of the following types:
```
char const*
unsigned long long
long double
char const*, std::size_t
wchar_t const*, std::size_t
char16_t const*, std::size_t
char32_t const*, std::size_t
```
I have shown you an example that takes a `long double` parameter, and an example that takes a `char const*` and a `std::size_t` parameters. But the **return value can be of any type**.
