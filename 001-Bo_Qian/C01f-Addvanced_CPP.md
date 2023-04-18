# Section 19 - Maintain is-a Relation For Public Inheritance
In the last section, we have talked about the ***public inheritance*** indicates the ***"is-a" relationship*** between the ***base class*** and the ***derived class***. So the ***derived class*** should be able to do everything the ***base class*** can do. Although the ***derived class*** can do those things in a different way, it has to offer some way to do this things to maintain the ***"is-a" relationship***. However, in real world programming, somtimes it is not so easy to maintain that ***"is-a" relationship***. 

For example, we have a class `Bird`, and we have another class `Penguin`, which is derived from `Bird`. And we know a `Bird` can fly, so in the class `Bird`, I can declare a method called `fly()`:
```
class Bird {
public:
    void fly();
};

class Penguin : public Bird {};
```
And in some application code, I can create `penguin`, and `penguin` can invoke the method `fly()` because it inherited from `Bird`. Now suddenly, we are making the `penguin` fly:
```
Penguin p;
p.fly();
```
So what is wrong? Here we are the victim of in-precise language English. When we say `Bird` can fly, we are not saying all `Bird` can fly. There are certain `Bird` that cannot fly, like `Penguin`. So the right way to do it is we need to define a new class called `FlyableBird`, which is derived from `Bird`. And the `Bird`'s `fly()` method certainly should belongs to the `FlyableBird` class. And now, `Penguin` is derived from `Bird` and it doesn't have to be able to fly:
```
class Bird {
};

class FlyableBird : public Bird {
public:
    void fly();
};

class Penguin : public Bird {};
```
So as you can see, there are subtle things that need to be taking care of to maintain ***"is-a" relationship***.

Now let's look at the second example. We have a class `Dog` and the `Dog` can barks `"I am just a dog."`. And the `YellowDog` is derived from `Dog`, and it barks `"I am a yellow dog."`. And in the `main()` function, I create a `YellowDog`, and let's it bark. And then I assign a `YellowDog`'s pointer to a `Dog`'s pointer. And then let it bark again:
```
class Dog {
public:
    void bark() {
        std::cout << "Whoof, I am just a dog.\n";
    }
};

class YellowDog : public Dog {
public:
    void bark() {
        std::cout << "Whoof, I am a yellow dog.\n";
    }
};

int main() {
    YellogDog* py = new YellowDog();
    py->bark();
    Dog* pd = py;
    pd->bark();
}
```
The output of this program is here:
```
Whoof, I am a yellow dog.
Whoof, I am just a dog.
```
So the same `Dog` calling the same method resulted in different behavior. This is not good. Some people may say *"This is polymorphism, you may have a problem with polymorphism"*. A ***polymorphism*** means a different classes of object react to the same API will behave differently. So the keyword there is different object react to same API behave differently. In this case, we have the same object react to the same API, and this behave differently. So this is **NOT** polymorphism. This is ***anti-polymorphism***.

So as much as we need a royal honest `Dog`, we need an object with consistent behavior in C++. So the conclusion we get from this example is **never overwrite non-virtual methods**. If there is a method that you really want to overwrite, make that method virtual.

Let's look at a different example. We've learned the lesson, we'll only overwrite virtual method. So the `Dog` has a virtual method `bark()`, and given the default value for message `msg` is `"just a"`. A `YellowDog` derived from `Dog`, and have a virtual method `Bark()` with the default value of message a `YellowDog`. In the `main()` function, I'm doing the exact same thing as previous example. I create a `YellowDog`, and let it `bark()`, and assign it to a `Dog`'s pointer, and let it `bark()` again. What's the output of this program:
```
class Dog {
public:
    virtual void bark(std::string msg = "just a") {
        std::cout << "Whoof, I am " << msg << " dog." << std::endl;
    }
};

class YellowDog : public Dog {
public:
    virtual void bark(std::string msg = "a yellow") {
        std::cout << "Whoof, I am " << msg << " dog." << std::endl;
    }
};

int main() {
    YellowDog* py = new YellowDog();
    py->bark();
    Dog* pd = py;
    pd->bark();
}
```
The output is also exactly the same as before, it will first bark `I am a yellow dog.`, and then bark `I am just a dog.`. So this is a cunning dog indeed:
```
Whoof, I am a yellow dog.
Whoof, I am just a dog.
```
We know that virtual method is bound at ***runtime***. However, the default value for function parameter is bound at compiler time. So when `pd` barks, it will invoke the virtual method of `YellowDog` as we expected. But it will pick up the default value of the message `msg` from `Dog`'s `bark()` method. That is why the `pd` barks `I am just a dog.`.

So the lesson we can take from this example is **never overwrite the default parameter value for virtual method**.

Now suppose the `Dog` has a different method `bark()`, which takes different parameter - integer `age`. And it prints out `"I am {{ age }} years old."`. In the `main()` function, `py` will bark at age `5`. And let's remove the `pd`:
```
class Dog {
public:
    void bark(int age) {                                            // here
        std::cout << "I am " << age << " years old" << std::endl;
    }
    virtual void bark(std::string msg = "just a") {
        std::cout << "Whoof, I am " << msg << " dog." << std::endl;
    }
};

class YellowDog : public Dog {
public:
    virtual void bark(std::string msg = "a yellow") {
        std::cout << "Whoof, I am " << msg << " dog." << std::endl;
    }
};

int main() {
    YellogDog* ps = new YellowDog();
    py->bark(5);                            // here
}
```
When the `py` bark with the parameter `5`, since `YellowDog` doesn't have a `bark()` method with integer parameter, you may think it should invoke the `Dog`'s `bark()` with integer, right? The answer is **NO**, this code won't even compile. The reason is when the compiler see the `bark()` method, do a first search `py` own class for a method with a name `bark()`, in this case, it will search `bark()` method inside the `YellowDog`. If the compiler cannot find the `bark()` method, it will continue searching in `Dog` class. 

However, **if the compiler did find a `bark()` method inside the `YellowDog`, regardless of the parameter and return value type, it will stop searching right there.** As a result, `YellowDog`'s `bark()` method is the only method that the compiler can see. So the two `bark()` methods of `Dog` are not inherited by `YellowDog`. They are shadowed by `YellowDog`'s own `bark()` method. This is not good, because the ***"is-a" relationship*** is broken. The `Dog` can `bark()` with integer, but the `YellowDog` cannot `bark()` with integer.

To maintain the ***"is-a" relationship***, one thing we could do is add this `using Dog::bark` statement. This will again bring the `Dog`'s `bark()` method back to `YellowDog`'s scope. And this code will compile:
```
...
class YellowDog : public Dog {
public:
    using Dog::bark;                                    // here
    virtual void bark(std::string msg = "a yellow") {
        std::cout << "Whoof, I am " << msg << " dog." << std::endl;
    }
};
...
```

In summary, to maintain the ***"is-a" relationship*** between the base class and the derived class. There are four things that we need to pay attention to:
1. Precise definition of classes.
   
2. Don't override non-virtual methods.
   
3. Don't override default parameter values for virtual functions.
   
4. Force inheritance of shadowed functions.




# Section 20 - Understanding *l-value* & *r-value*
We're going to talk about ***l-value*** and ***r-value***.

***l-value*** and ***r-value*** are not something that learn into very often. Some people have programmed C++ for years, and they still don't have good understanding of what the ***l-value*** and the ***r-value*** are.

So the question to ask is, why do I care? ***l-value*** and ***r-value*** are actually very important concept in C++ core language. Having a good knowledge of them will help you understand C++ construct, and to explain some simliar strang behavior of C++. It also can help you to decipher compiler errors and warnings, because these two terms appears quite often in the compiler messages.

A second reason that you should care about them is ***C++11*** introduce the new feature called ***r-value reference***. If you don't have a good grasp of ***r-value*** and ***l-value***, it will be impossible for you to learn ***r-value reference***.


### Definition of *l-value* & *r-value*
What are the ***l-value*** and ***r-value***? It is hard to give a rigorous definition of what they are. Instead, here I am give you a simplified definition, which is generally accepted and it should serve you well in the 99% of the time.

- ***l-value*** is an object that occupies some identifiable location in memory. So the keyword here is something in ***memory***, not something in the ***register***. Secondly it has an ***identifiable address***.
  
- ***r-value*** is defined by exclusion, any object that is not a ***l-value***, is a ***r-value***.


### *l-value* Examples
Now let's look at some ***l-value* examples**.

I have an integer `i`, `i` is a ***l-value***. Why it is a ***l-value***? Because I can get its address with **ampersand sign `&`**, and assign that address to an ***integer pointer* `p`**. So not only it has an address, it has an address that is **identifiable**. And the content of that address can be modified by assigning `i` to a different value:
```
int i;              // i is a l-value
int* p = &i;        // i's address is identifiable
i = 2;              // memory content is modified
```

Second example is I have a class `Dog` and then I create a `Dog` object `d1`, and `d1` is a ***l-value*** of a ***user-defined type* - `class`**:
```
class Dog;
Dog d1;             // l-value of user defined type (class)
```
It is fair to say that most of the variables in C++ code are ***l-values***.


### *r-value* Examples
Now let's look at some ***r-value* examples**.

I have an integer `x` and initialize it with `2`. As we said, `x` is a ***l-value***. And `2` is a ***r-value***:
```
int x = 2;              // 2 is an r-value
```

In this case, `x` is initialized with `i + 2`. `i + 2` is an ***r-value***:
```
int x = i + 2;          // (i + 2) is an r-value
```

Why they are ***r-value***? Because if I try to get their address like this, the compiler will error out:
```
int* p = &(i + 2);      // Error
```

And I also cannot assign a different value to them like this. So they are ***r-values***:
```
i + 2 = 4;              // Error
2 = i;                  // Error
```

In this example, I have a `Dog` object `d1`, and the reture value of `Dog()` is assigned to `d1`. `Dog()` is a ***r-value*** of ***user-defined type* - `class`**.
```
Dog d1;
d1 = Dog();             // Dog() is r-value of user defined type (class)
```

Now let's look at a function, I have a function `sum()`, which takes an integer `x` and an integer `y`. And then return the sum of `x` and `y`. And then I have a variable `i` and initialize it with the return value of `sum(3, 4)`. `sum(3, 4)` is the ***r-value***:
```
int sum(int x, int y) {
    return x + y;
}
int i = sum(3, 4);      // sum(3, 4) is r-value
```

To conclude, here are the ***l-values*** in the above examples:
```
l-values: x, i, d1, p
```

And here are the r-value in the above examples:
```
r-values: 2, i + 2, Dog(), sum(3, 4), x + y
```

Let's look at the reference. I assume you are ready familiar with ***reference*** in C++.

Here I have created an integer `i`, and I initialize an integer reference `r` with `i`. This reference should actually be called ***l-value reference***, because `r` is a reference to our ***l-value* `i`**:
```
int i;
int& r = i;
```

If I initialize the **reference `r`** with a ***r-value* `5`**, then the compiler will error out:
```
int& r = 5;     // Error
```

The only exception of this is if `r` is a `const` reference, then it can be assigned with `5`. Although you really should consider this is a shortcut of a ***two-step operations***. ***Step 1***, a **temporary *l-value*** is created with `5`. ***Step 2***, the reference `r` is initialized with this ***temporary l-value***:
```
const int& r = 5;
```

Now let's look at the same idea with the function. I have a function `square()`, which takes a ***l-value reference* `x`**, and it returns **`x`<sup>2</sup>**:
```
int square(int& x) {
    return x * x;
}
```

When I invoke the function `square(i)`, `i` is an ***integer***, and it is okay.
```
square(i);                      // OK
```

If I invoke the function `square(40)`, then it is an error because `40` is not a ***l-value***, it's a ***r-value***:
```
square(40);                     // Error
```

How to make `square(40)` work? The workaround is I can change the parameter of the `square()` function into a **`const` integer reference**. And we have seen in the previous example, a **`const` integer reference** can be initialized with a ***r-value***. So the `square(40)` will work, and `square(i)` will still work:
```
int square(const int& x) {
    return x * x;               // square(40) and square(i) work
}

square(40);                     // OK
square(i);                      // OK
```

What confuse the most of the people in the transforming between the ***l-value*** and ***r-value***? An ***l-value*** can be used to create an ***r-value***. In this example, I have a **l-value `i`** and I can create a **r-value** with `i + 2`:
```
int i = 1;
int x = i + 2;
```


### Implicitly Transform *l-value* to *r-value*
In the second example, integer `x` is initialized with `i`. Now, what is `i`? Is it ***r-value***, or ***l-value***? `i` is obviously a ***l-value***, because it has an address and the `i`'s address is identifiable. However, in the second statement, **`i` is implicitly transformed to a *r-value***:
```
int i = 1;
int x = i;
```
So a ***l-value*** can be implicitly transformed into a ***r-value***. However, the other way around is not valid, and ***r-value*** cannot be implicitly transformed into a ***l-value***. And ***r-value*** should be explicitly used to create our ***l-value***. r-value should be explicitly used to create a ***l-value***.


### Dereferenced *r-value* Can Be Used as *l-value*
Here is an example of using ***r-value*** to create ***l-value***. I have an integer array `v`, and `v + 2` is our ***r-value***. However, the dereference of `v + 2` is a `l-value`, which can be assigned with a different value:
```
int v[3];
*(v + 2) = 4;
```


### Misconception about *l-values* & *r-values*
Instead of continue to talk about what the ***r-values*** and ***l-values*** are. Let's talk about what they are not. There are some misconception that needs to be cleared up.

- #### Misconception 1 - Function or Operator Always Yields *r-values*
This conclusion can be easily drawn from the example of where that we've seen so far. In the first statement, initialize `x` with `i + 3`, `i + 3` is a ***r-value***. And in the second statement, `sum(3, 4)` is a ***r-value***:
```
int x = i + 3;
int y = sum(3, 4);
```

However, this conclusion is **NOT** right. Here is my counter examples. I have a function `foo()`, which returns an integer reference. And in the function, I returns a global variable `myGlobal` by reference. So the return value from the function `foo` is a ***l-value***. I can write code `foo() = 50`, this will complile:
```
int myGlobal;
int& foo() {
    return myGlobal;
}
foo() = 50;         // `foo()` is not r-value
```
Some of you may say *"Well, this is a weird-looking code. I never write code like this"*.

Well, point taken, then how about this example? In this example, I'm invoking subscript operator `operator[]` on an array. And `operator[]` almost always generates ***l-value***.
```
array[3] = 50;      // operator[] always generates l-value
```

- #### Misconception 2 - *l-value* are Always Modifiable
In ***C***, ***l-value*** is defined as a value suitable for left-hand side of assignment. However, this definition is no longer true in C++, because C++ has a `const`, and a `const` although is a ***l-value***, it is **NOT** modifiable.
```
const int c = 1;
c = 2;              // Error, `c` is not modifiable
```

- #### Misconception 3 - *r-values* are not modifiable
This seems to be pretty obvious in our examples. `i + 3` is ***r-value***, and it's not modifiable. `sum(3, 4)` is a ***r-value***, and it's not modifiable:
```
i + 3 = 6;          // Error
sum(3, 4) = 7;      // Error
```

However, this conclusion is only true for the ***built-in types***. It is not true for **user-defined type or `class`**. For example, I can call the default constructor of `Dog` to create a `Dog` object, and then invoke the method `.bark()` on the `Dog` object. The method `.bark()` could change the state of this `Dog()`. So this r-value `Dog()` is modifiable:
```
class Dog;
Dog().bark();       // bark() may change the state of the `Dog` object
```

### Summary
If you cannot remember everything that I've talked about ***r-value*** and ***l-value***, please do remember the summary.
1. Every C++ expression yield either an ***r-value*** or a ***l-value***.
2. If the expression has an ***identifiable memory address***, it's a ***l-value***. Otherwise, it's a ***r-value***.




# Section 21 - Static Polymorphism
We're going to talk about ***static polymorphism***. When we talk about ***polymorphism***, by default, we are talking about ***dynamic polymorphism***, because ***dynamic polymorphism*** is such an important part of ***object-oriented programming***.

Here I am giving you an example of ***dynamic polymorphism***. This is an example of tree parsing. We have a class `Generic_Parser`, and `Generic_Parser` has a publlic member method `parse_preorder()`, which does the preorder parsing of the tree. And while passing on the tree, it invoke a private member method `process_node()`, which does certain specific things to this particular node. However, the `Generic_Parser`'s `process_node()` method is an empty method, which does nothing. `EmployeeChart_Parser` is a specialized parser, and it's derived from the `Generic_Parser`. It overwrites the `process_node()` method, and it doesn't customize thing for the ***employee chart***. In the `main()` function, an `EmployeeChart_Parser` object `ep` is created, and the `ep` will call the `.parse_preorder()` method from its base class. The `.parse_preorder()` method will call `.process_node()` becasue the `.process_node()` method is a virtual method. So it will actually call the `EmployeeChart_Parser`'s `process_node()`. So the generic algorithm of `parse_preorder()` is customized by `EmployeeChart_Parser` own `process_node()` method. This is a typical example of polymorphism:
```
struct TreeNode {
    TreeNode* left, *right;
};

class Generic_Parser {
public:
    void parse_preorder(TreeNode* node) {
        if(node) {
            process_node(node);
            parse_preorder(node->left);
            parse_preorder(node->right);
        }
    }
private:
    virtual void process_node(TreeNode* node) {}
};

class EmployeeChart_Parser : public Generic_Parser {
private:
    void process_node(TreeNode* node) {
        std::cout << "Customized process_node() for EmployeeChart.\n";
    }
};

int main() {
    ...
    EmployeeChart_Parser ep;
    ep.parse_preorder(root);
}
```
We like polymorphism because it makes your code more clear and elegant. It helps you write more generic code that is more decouple from other code. However, polymorphism is not for free. It comes with a small price to pay. The price is:
1. The memory cost of the ***virtual table***.
2. The runtime cost of ***dynamic binding***, which is basically the code that creates and uses the ***virtual table***.

This cost us a small. However, what if my profile tells me that this is a critcial part of my algorithm, and it needs to run as fast as it can, and I don't want to give up on the benefit of ***polymorphism***. What can I do?

One solution that I'm going to show you in this section is we can actually simulate the behavior of polymorphism. By simulating it, we're getting its benefit, and then not paying its price.

Before going to the solution, let's take a look of what are the things that we want to simulate:
1. ***"is-a" relationship*** between **base class** and **derived class**. So the `EmployeeChart_Parser` class is publicly derived from `Generic_Parser` class.
   
2. Base class defines a "generic" algorthim that is used by ***derived class***. In this case, the generic algorithm is the `parse_preorder()` method.
   
3. The "generic" algorthim is customized by the derived class. In this example, that is done with the `process_node()` method.

Having these three things in mind. Let's look at our solution. The first thing you might notice is the base class `Generic_Parser` now has become a template class of type `T`. `parse_preorder()` method is the :
```
template<typename T> class Generic_Parser {
public:
    void parse_preorder(TreeNode* node) {
        if(node) {
            process_node(node);
            parse_preorder(node->left);
            parse_preorder(node->right);
        }
    }
    void process_node(TreeNode* node) {
        static_cast<T*>(this)->process_node(node);
    }
};

class EmployeeChart_Parser : public Generic_Parser<EmployeeChart_Parser> {
public:
    void process_node(TreeNode* node) {
        std::cout << "Customized process_node() for EmployeeChart.\n";
    }
};
```

# 21 - 4:10