# Section 01 - `const`
`const` is a compile time constraint that an object cannot be modified. This constrait is enforced at compile time.

Here we have a `const` integer, and if we want to change `i` to `6`. It failed with the message: `assignment of read-only variable 'i'`:
```
const int i = 9;
i = 6;              // error: assignment of read-only variable 'i'
```
So this is what a constant ask. It enforces the "constness" during the compile time.


### Define Pointed Data Constant
Now let's look at some more examples. In this case, the data that point to by `p1` is a `const`, but the pointer itself, `p1` is not `const`. So if we chnage the data that `p1` point to, it will not compile, because the assignment of read-only location `*p1`:
```
const int* p1 = &i;     // data is 'const', pointer is not 'const'
*p1 = 5;                // error: assignment of read-only location '*p1'
```
However, if we increment the pointer itself using pointer arithmetic. That modify the `p1` itself. It runs through okay:
```
const int* p1 = &i;
p1++;                   // valid statement
```
So `p1` is not a constant, but the data pointed by `p1` is a constant. The way to remember it is **by reading `const int`, so the integer is a constant**.


### Define Pointer Constant
In this example, this is the opposite of `p1`, which means the `p2` itself is a constant, but the data it points to not a constant:
```
int* const p2;          // pointer is 'const', data is not
```
So the way to remember it is `const p2`, so `p2` is a `const`.


### Define Both Data & Pointer Constant
Now, both the pointer `p3` and the data pointed to by `p3` are `const`:
```
const int* const p3;    // data and pointer are both 'const'
```


### Rule to Define Constant Pointed Data & Constant Pointer
So, it looks easy to remember, `const int`, the integer is `const`. `const p2`, the `p2` is `const`. But here is the tricky one, in this case, so what is `const` and what is not `const`:
```
int const* p4 = &i;
```
**The rule that you should remember is this:**
- **If `const` is on the *left* of `*`, *data* is `const`**
- **If `const` is on the *right* of `*`, *pointer* is `const`**

It is best practice to rewrite this code by changing the order of `int` and `const` to avoid confusion:
```
const int* p4 = &i;
```
Since in both case, the `const` is on the left of the `*`, so they are the same. And `const int* p4` is much easier to make sense of. `const int`, the integer is `const`. So in this case, the data is `const`, the pointer is not.


### Constant Can Be Cast Away by using `const_cast` Operator
In C++, the type of data can be casted from one to another, and **the "constness" of a data can also be casted away**. In this case, we know the second statement will not compile because we're changing a `const` data:
```
const int i = 9;
i = 6;                      // Compiler error
```
Say we really really want to change the integer `i`, we can use `const_cast` operator to case away the "constness" of `i`:
```
const int i = 9;
const_cast<int&>(i) = 6;    // valid statement
```
`const_const` is an ***operator*** in C++. It is used to remove the `const` and/or `volatile` qualifiers from a variable.


### Make Variable a Constant by using `static_cast` Operator
Say we have an integer `j`, and `j` is not a `const`. So we can change it to any value we want. But at a certain point, we want to make `j` a `const`. We can do this by using `static_cast` operator. This will make `j` a constant integer:
```
int j = 9;
static_cast<const int&>(j);
```
If we modify its value to `7`, it failed:
```
int j = 9;
static_cast<const int&>(j);     // error: assignment of read-only location '*(const int*)(& j)'
```

`static_case` is a C++ operator used to convert between different types in a type-safe manner. `static_cast` can also be used for ***pointer conversions***, ***upcasting*** and ***downcasting*** in class hierarchies, and converting between arithmetic types.

It is important to note that `static_cast` is a ***compile-time operator*** and **does not perform any runtime checks**. Therefore, it should be used with caution and only when the programmer is certain that the conversion is safe.


### Be Careful Of Using Cast with Constant
So with ***cast***, we can cast away the "constness" of data, and we can also cast the data into a `const`. But in general, ***cast*** is not a good thing. You should avoid them as much as possible.

By cast away the "constness" of a data, we're breaking someone's promise for that data to be a `const`. So ***cast*** is a hacky way of coding.


### Benefit of Using `const`
The beginners usually don't like `const`. It looks nothing but a troublemaker. When the code doesn't compile, remove the `const`, and it compiles. However, `const` is really a good thing to use. It has several benefits.

First, `const` guards against inadvertent write to the variable, so it can stop the wrong behavior at the compile time, rather than wait until the runtime.

Secondly, `const` is a way of self-documenting. By using `const`, you are telling your reader that this variable will not changed, and that is a extra inforamtion for your readers to understand the code.

Thirdly, `const` enables the compiler to do more optimization, so the compiler can make the code tighter, and therefore faster.

Lastly, `const` also means that the variable can be put in **ROM**, the ***Read-Only Memory***. This is particularly useful in the embedded programming.

So, as a professional C++ programmer, you should be using `const` proactively.




# Section 02 - `const` and Functions
Say we have a class `Dog`, which has an integer of `age`, and a string of `name`. And `Dog` has a constructor that initalized the `age` and `name`. And `Dog` has a method called `.setAge()`, and which set `age` to the parameter of `a`. And in the `main()` function, we create the `Dog` - `d`, and we have an integer `i` and we have an integer `i` set to `9`, and then we called `d.setAge(i)`, and then print out the `i`:
```
class Dag {
    int age;
    std::string name;
public:
    Dog() {
        age = 3;
        name = "dummy";
    }

    void setAge(int& a) {
        age = a;
    }
}

int main() {
    Dog d;

    int i = 9;
    d.setAge(i);
    std::cout << i << std::endl;
}
```

Suppose the `.setAge()` method not only use `a` to set `age`. It also make some change to the parameter `a`, say increment `a` by `1`. Remember, the `.setAge()` method takes the parameter by reference, so when we increment `a`, it also incremented the original varaible `i`. So if we run the program, it prints out `10`, because `i` has been changed to `10` by the set age function.:
```
class Dag {
...
    void setAge(int& a) {
        age = a;
        a++;                        // increment `a` by 1
    }
}

int main() {
    ...
    int i = 9;
    d.setAge(i);
    std::cout << i << std::endl;    // 10
}
```


### `const` Reference Parameter
Say we don't want that to happen, `i` is a local variable in the `main()` function, and we don't want the `.setAge()` method to change it. What we can do is add a `const` over here. Now we're passing `i` as a `const` integer reference to the `.setAge()` method. So if we compile the program, it will fail:
```
class Dag {
...
    void setAge(const int& a) {     // add `const` here
        age = a;
        a++;
    }
}

int main() {
    ...
    int i = 9;
    d.setAge(i);                    // Compiler error
    std::cout << i << std::endl;
}
```
If we remove `a++`, it will succeed:
```
    void setAge(const int& a) {     // add `const` here
        age = a;
    }
```
So this is how a `const` reference is passed as a parameter to a function. `const` reference parameter is widely used in C++ functions, and you should be using it whenever it is appropriate.

Now consider if we remove the reference and the `.setAge()` method only takes a `const` integer as a parameter. In this case, the `const` is not very useful. It still means that `a` cannot be changed inside the function, but it is not as useful as it is used as `const` reference parameter, because the parameter is **passed by value**, so in the `main()` function when we call `.setAge(i)`, we're making a copy of `i` and pass that to the method `.setAge()`. And whether the function `.setAge()` will use that copy as a `const` and non-`const`, we don't care, it's a copy anyway, do whenever you want:
```
    void setAge(const int a) {      // passed by value
        age = a;
    }
```
So from the caller's point of view, this `const` is useless. If we overloaded this method with another function that takes integer as a parameter, the caller cannot differentiate these two functions. So if we compile it, it will fail and with the message that the `.setAge()` cannot be overloaded. So from the caller's point, these two methods are the same:
```
class Dag {
...
    void setAge(const int a) {
        age = a;
    }
    void setAge(int a) {
        age = a;
    }
}

// error: 'void Dog::setAge(int)' cannot be overloaded
```


### `const` Return Value
Now let's look at an example of **`const` return value**. Here we have a method `.getName()`, which returns a **`const` string reference**. And the reason we want to return a **`const` string reference** is the same reason why we want a **`const` reference parameter**. We want to return by reference to improve the efficiency of the program, and we want to return a `const` reference so that the caller cannot change the variable that we returned. In the `main()` function, we'll assign the return value of `d.getName()` to a `const` string reference `n`, and then print it out:
```
class Dog {
    ...
    Dog() { 
        age = 3;
        name = "dummy";
    }
    ...
    const std::string& getName() {
        return name;
    }
}

int main() {
    Dog d;
    const std::string& n = d.getName();
    std::cout << n << std::endl;
}
```
If we run the program, `"dummy"` is printed out. So this is how we use a `const` reference return value.

Let's consider remove reference and return `const` string. Now the `const` is completely useless, because the `name` is return by value. So what the function return is the copy of the `name`, which is a ***temporary***. And it doesn't make any sense for a temporary to be defined as `const`:
```
class Dog {
    ...
    const std::string getName() {
        return name;
    }
}
```


### `const` Functions
Now let's consider a more interesting case, `const` function. We have a method called `.printDogName()`, and then after the function signature, there is a `const`. This means **this method will not change any of the member variables of this class**:
```
class Dog {
    ...
    Dog() { 
        age = 3;
        name = "dummy";
    }
    ...
    void printDogName() const {
        std::cout << name << "const" << std::endl;
    }
}
```
So in the `main()` function, we could called `d.printDogName()`:
```
int main() {
    Dog d;
    
    d.printDogName();
}
```
Let's run it, and it prints out `dummyconst`.

However, if the method change the member variables, says `age++`. This couldn't compile:
```
    void printDogName() const {
        std::cout << name << "const" << std::endl;
        age++;
    }
```
And another thing that you should remember is even if this method doesn't change any of the member variables but it call another member method, let's `.getName()`, **which is not a `const` function**, **this still doesn't compile** even though the `.getName()` method doesn't change the member variables at all:
```
    ...
    const string& getName() {                               // non-`const` function
        return name;
    }

    void printDogName() const {
        std::cout << getName() << "const" << std::endl;     // Compiler error
    }
```
So **a `const` function can only call another `const` function** in order to the `const` correctness.


### Overloading `const` Function
Another thing to know is "constness" can be used to overload the function:
```
    ...
    void printDogName() const {
        std::cout << name << "const" << std::endl;
    }

    void printDogName() {
        std::cout << getName() << "non-const" << std::endl;
    }
    ...
```
Now the question is which function will be invoked. The answer is the `const` version of `printDogName()` will be invoked if the `Dog` object is a `const`. And the non-`const` version of `printDogName()` will be invoked if the `Dog` object is not a `const`. For example, we have a `Dog` object `d`. This `d` should call the non-`const` version of the `printDogName()`. The `d2` should call the `const` version of `printDogName()`:
```
int main() {
    Dog d;
    d.printDogName();       // non-const version called

    const Dog d2;
    d2.printDogName();      // const version called
}
```
So this is how a `const` function can be overloaded with a non-`const` function.


### Overloading `const` Reference Parameter
The last thing to know is a function that takes a **`const` reference parameter** can also be overloaded the function that takes a **reference parameter**:
```
class Dog{
    ...
    void setAge(const int& a) {
        age = a;
    }
    void setAge(int& a) {
        age = a;
    }
}
```
