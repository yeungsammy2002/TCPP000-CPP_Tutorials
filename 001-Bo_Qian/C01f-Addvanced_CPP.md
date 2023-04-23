# Section 19 - Maintain *"is-a"* Relation For Public Inheritance
In the last section, we have talked about the ***public inheritance*** indicates the ***"is-a" relationship*** between the ***base class*** and the ***derived class***. So the ***derived class*** should be able to do everything the ***base class*** can do. Although the ***derived class*** can do those things in a different way, it has to offer some way to do this things to maintain the ***"is-a" relationship***. However, in real world programming, sometimes it is not so easy to maintain that ***"is-a" relationship***. 



## Maintain *"is-a"* Relationship Semantically
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



## Never Overwrite Non-Virtual Methods
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



## Never Overwrite Default Parameter Value for Virtual Methods
Let's look at a different example. We've learned the lesson, we'll only overwrite virtual method. So the `Dog` has a virtual method `bark()`, and given the default value for message `msg` is `"just a"`. A `YellowDog` derived from `Dog`, and have a virtual method `Bark()` with the default value of message `msg` - `"a YellowDog"`. In the `main()` function, I'm doing the exact same thing as previous example. I create a `YellowDog`, and let it `bark()`, and assign it to a `Dog`'s pointer, and let it `bark()` again. What's the output of this program:
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



## Parent's Overloading Methods are Shadowed by Child's Methods with Same Name
Now suppose the `Dog` has a different method `bark()` **\*here**, which takes different parameter - integer `age`. And it prints out `"I am {{ age }} years old."`. In the `main()` function, `py` will bark at age `5`, and let's remove the `pd`:
```
class Dog {
public:
    void bark(int age) {                     // *here
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
    py->bark(5);                            // *here2
}
```
When the `py` bark with the parameter `5` **\*here2**, since `YellowDog` doesn't have a `bark()` method with integer parameter, you may think it should invoke the `Dog`'s `bark()` with integer, right? The answer is **NO**, this code won't even compile. The reason is when the compiler see the `bark()` method, do a first search `py` own class for a method with a name `bark()`, in this case, it will search `bark()` method inside the `YellowDog`. If the compiler cannot find the `bark()` method, it will continue searching in `Dog` class. 

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



## Summary
In summary, to maintain the ***"is-a" relationship*** between the base class and the derived class. There are four things that we need to pay attention to:
1. Precise definition of classes.
   
2. Don't override non-virtual methods.
   
3. Don't override default parameter values for virtual functions.
   
4. Force inheritance of shadowed functions.




# Section 20 - Understanding *l-value* & *r-value*
We're going to talk about ***l-value*** and ***r-value***.

***l-value*** and ***r-value*** are not something that learn into very often. Some people have programmed C++ for years, but they still don't have good understanding of what the ***l-value*** and the ***r-value*** are.

So the question to ask is, why do I care? ***l-value*** and ***r-value*** are actually very important concept in ***C++ core language***. Having a good knowledge of them will help you understand C++ construct, and to explain some simliar strange behavior of C++. It also can help you to decipher compiler errors and warnings, because these two terms appear quite often in the compiler messages.

A second reason that you should care about them is ***C++11*** introduce a new feature called ***r-value reference***. If you don't have a good grasp of ***r-value*** and ***l-value***, it will be impossible for you to learn ***r-value reference***.


### Definition of *l-value* & *r-value*
What are the ***l-value*** and ***r-value***? It is hard to give a rigorous definition of what they are. Instead, here I am giving you a simplified definition, which is generally accepted and it should serve you well in the 99% of the time.

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

Having these three things in mind. Let's look at our solution. The first thing you might notice is the base class `Generic_Parser` now has become a template class of type `T`. `parse_preorder()` method is the same as before. However, the `process_node()` method is changed. Instead of doing nothing like before, it will statically casted `this` object into the type of `T`. And it invoke the `process_node()` method of the type `T`. Now here is the tricky part, the `EmployeeChart_Parser` class is publicly derived from `Generic_Parser`, and then the `Generic_Parser` is a template class with a template type of `EmployeeChart_Parser`. The rest of the code is the same, the `process_node()` will do some customize thing for the ***employee chart***. In the `main()` function is also the same, it will create an `EmployeeChart_Parser` object and invoke the `parse_preorder()` method for the ***employee chart***. The `.parse_preorder()` method is the method of the `Generic_Parser` **\*overhere**. It will invoke the `process_node()` method. Please note that the `.process_node()` method is no longer a virtual method. So the `Generic_Parser` will call itself a `.process_node()` method, not the `EmployeeChart_Parser`'s `process_node()` method. However, the `Generic_Parser`'s `process_node()` method will case `this` object into type of `T` and invoke `T`'s `process_node()` method. In this example, the type `T` is actually `EmployeeChart_Parser`. So eventually, it is `EmployeeChart_Parser`'s `process_node()` method that got invoked. Thus the polymorphism is achieved:
```
template<typename T> class Generic_Parser {
public:
    void parse_preorder(TreeNode* node) {           // *overhere
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

int main() {
    ...
    EmployeeChart_Parser ep;
    ep.parse_preorder(root);
    ...
}
```
Now let's review the three elements that we want to simulate:
1. The ***"is-a" relationship*** between the ***base class*** and the ***derived class***. That is still true.
   
2. ***Base class*** defines a ***"generic" algorithm*** that is used by ***derived class***, which is also true.
   
3. The ***"generic" algorithm*** is customized by the ***derived class***. And here is done by the `process_node()`, it's also true.

So we're getting all the benefits of polymorphism, but we're not paying any price for it. Another thing to note is from our kind point of view, they can use our class as if it is a true polymorphism. They don't even care whether it is real or simulated polymorphism. This is called ***curiously recurring template pattern* - *CRTP***, is also called ***static polymorphism***, or ***simulated polymorphism***. It is very popular in the library code, because in the most of the application code, you don't really care about the cost of the ***virtual table*** and the ***dynamic binding*** unless your profile tell you that is important. But in the library code, often time you want to squeeze every bit of performance out of your code.

So we are getting all the benefits of polymorphism, but we are not paying price for it. It's like a free launch, right? No, nothing is free in this world. Everything comes in with the price. So what is the price we are paying here?

Suppose I create another parser, which is called `MilitaryChart_Parser`, and the `MilitaryChart_Parser` will be derived from the `Generic_Parser` of the template type of `MilitaryChart_Parser`. Note that the `Generic_Parser` of `EmployeeChart_Parser` and the `Generic_Parser` of `MilitaryChart_Parser` are two different classes. They are distinguished classes that occupies their own space in the program image. Now you realized that the launch is not free. This is a typical trade off between a program image size and a program performance. Whether the trade off is worth it, it totally depends on your application:
```
int main() {
    ...
    MilitaryChart_Parser ep;
    ...
}
```
Another thing is want to point out is this is also a small demo of ***Template Metaprogramming* - *TMP***. The idea of ***template metaprogramming*** is it moves part of computation, which typical happens during ***runtime*** up front to the ***compile time***. Therefore, improve the efficiency of your program. And that is exactly what our ***static polymorphism*** does, although ***static polymorphism*** only improve the efficiency a little bit. Sometimes, by using the ***TMP technique***, you can make much bigger improvements to your program.

The last thing that I want to point out is when some people talk about ***static polymorphism***, they are actually referring to the template itself. In this example, I have a template function `Max()`, which go through every element of the vector `v` and find the largest one and return it:
```
template<typename T>
T Max(std::vector<T> v) {
    T max = v[0];
    for(typname std::vector<T>::iterator it = v.begin(); it != v.end(); ++it) {
        it(*it > max)
            ret = *it;
    }
    return max;
}
```
When function `Max()` is materialized with any different type of `T`, or the operators, such as larger than `>`, the copy assignment `=`, will be invoked with different version of ***operator larger than* - `operator>`** and ***copy assignment* - `operator=(const T&)`**. And because that happens in the compiler time, they called it ***static polymorphism***. This nothing wrong with it. It's just a different definition. I just want to clear up with the concepts, so next time you hear other people talking about ***static polymorphism***, you know which one they are talking about.




# Section 22 - Multiple Inheritance - Devil or Angel
We're going to talk about ***multiple inheritance***. ***Multiple inheritance*** is where **a class is directly derived from more than one *base classes***. Poeple have different opinions with the ***multiple inheritance***. Some people says if ***single inheritance*** is good, then ***multiple inheritance*** must be better. And some people says ***multiple inheritance*** brings too much trouble and subtlety. And its benefits are not worth by the trouble it has made. So I'm going to show you all the benefits and the subtlety with ***multiple inheritance***. In the end, I'll layout my position on this and you decide for your own position.

Let's look at an example. We a class `InputFile`, which opens a file to read. So it has a public `read()` method. We have an `OutputFile` class, it open the file to write, and of course, it has a `write()` method. And later on, it turns out I need to open a file for both reading and writing. It makes sense that we create another class called `IOFile`, which inherit both `InputFile` and `OutputFile`. And as a result, `IOFile` can do both ***reading*** and ***writing***. So far so good, multiple inheritance has served its purpose. Now in order to read a file, we must open the file first. So the `InputFile` needs another method called `open()`. And for the same reason, the `OutputFile` also need a method `open()`. In the `main()` function, I create an `IOFile` object `f`, and I call `f.open()`:
```
class InputFile {
public:
    void read();
    void open();
};

class OutputFile {
public:
    void write();
    void open();
};

class IOFile : public InputFile, public OutputFile {
};

int main() {
    IOFile f;
    f.open();
}
```
This code will not compile, because `IOFile` has two instances of the `open()` method. One `open()` is inherited from `InputFile`, and another `open()` is inherited from `OutputFile`. So the compiler will issue an error saying *"I don't know which open() you want to call."*. What's even worse is if the `InputFile` has a private `open()` method:
```
class InputFile {
public:
    void read();
private:
    void open();
};
...
```
Now the `IOFile` has access to only one method of `open()`. So you may think the `f.open()` should invoke the `OutputFile`'s `open()`method, right? No, this still will not compiler.

The C++ standard says **before the compiler sees accessiblility of a function, it must decide which function is the best match for the function call.** So before the compiler see that this `InputFile`'s `open()` method is ***private***. It must first determine which `open()` method should be invoked. That's why the compiler will still issue an error saying *"ambiguous call to the `open()` method"*.

To open the file successfully, you have to say `f.OutputFile::open()`, that would specifically tell the compiler that you want to use the `OutputFile`'s `open()` method to open the file. Now you have seen one subtlety with the ***multiple inheritance***. And let's see more.

Since both `InputFile` and `OutputFile` have the `open()` method, it makes sense for the `InputFile` and `OutputFile` to have a ***common base class***, and move the `open()` method to that ***common base class***. And that is what we are going to do.


### Diamand Shape Problem
We have a `File` class, and the `File` have a `name` of string, and also the `open()` method. Both `InputFile` and `OutputFile` are derived from `File`, and `IOFile` is derived from both `InputFile` and `OutputFile`:
```
class File {                //          File
public:                     //          /  \
    std::string name;       //  InputFile  OutputFile
    void open();            //          \  /
};                          //         IOFile

class InputFile : public File {
};

class OutputFile : public File {
};

class IOFile : public InputFile, public OutputFile {
};      // Diamond shape of hierarchy

int main() {
    IOFile f;
    f.open();
}
```
Now in the `main()` function, if I `IOFile`'s `open()` method, you may think it should be okay, because we only have one `open()` method available. The compiler will still issue an error and saying *"ambiguous call to the `open()` method"*. The reason is the `InputFile` will inherit the instance of `File`'s `open()` method, and the `OutputFile` will also inherit the instance of `File`'s `open()` method. So the `IOFile` still ends up having two instances of `open()` method. One from the `InputFile`, and another one from the `OutputFile`. That is why the compiler will issue an error saying *"ambiguous call to the `open()` method"*. This is commonly known as a ***diamond shape problem***. Because the classes form a diamond shap of hierarchy:
```
          File
          /  \
  InputFile  OutputFile
          \  /
         IOFile
```


### Virtual Inheritance
The `IOFile` not only has two instances of `open()` method, it also has two instances of `name` and I can assign a different value to the two names. For example, I can do `f.InputFile::name = "File1"` and `f.OutputFile::name = "File2"`. So this is a problem, why do need two instances of `open()` and two instances of `name`. How to fix this?
```
...
int main() {
    IOFile f;
    f.open();
    f.InputFile::name = "File1";
    f.OutputFile::name = "File2";
}
```

C++ provide a solution called ***virtual inheritance***. I can virtually inherit the `InputFile` from the `File` and virtually inherit `OutputFile` from the `File`. What that means is we're telling the compiler we only need one instance of `name` and one instance of `open()` method during the process of inheritance. And as a result, this two lines of code `f.InputFile::name = "File1"` and `f.OutputFile::name = "File2"` will not compile:
```
class File {
public:
    std::string name;
    void open();
};  

class InputFile : virtual public File {         // here
};

class OutputFile : virtual public File {        // here
};

class IOFile : public InputFile, public OutputFile {
};      // Diamond shape of hierarchy

int main() {
    IOFile f;
    f.open();
    f.InputFile::name = "File1";                // Error
    f.OutputFile::name = "File2";               // Error
}
```
But the `f.open()` will compile successfully:
```
...
int main() {
    IOFile f;
    f.open();
}
```
Okay, the virtual base class is good, it solves our problem. However, it introduces another problem, which is the problem of ***initialization***.

Suppose we have defined a constructor for `File`, which takes a string parameter of file name `fname`. Since we have defined a ***non default constructor*** for `File`, the compiler will not generate the ***default constructor*** for us, which means all the `File`'s children have to explicitly initialize the `File` in their constructor. And the `IOFile` will initialize both `OutputFile` and `InputFile`. This will work out okay, if it is a ***single inheritance***. When I create an `IOFile` object `f`, `IOFile` will initialize an `OutputFile` object, which in turns initialize a `File` object. However, this is a problem for ***multiple inheritance***:
```
class File {
public:
    File(std::string fname);
};

class InputFile : virtual public File {
    InputFile(std::string fname) : File(fname) {}
};

class OutputFile : virtual public File {
    OutputFile(std::string fname) : File(fname) {}
};

class IOFile : public InputFile, public OutputFile {
    IOFile(std::string fname) : OutputFile(fname), InputFile(fname) {}
};

int main() {
    IOFile f;
}
```
When `IOFile` is derived from both the `InputFile` and the `OutputFile`, it needs to initialize both `InputFile` and `OutputFile`, which in turns initialize two instances of `File`. That is bad. And C++ provides a solution by defining a rule, the rule states that the **initialization of the *base virtual class*** is the responsibility of ***the most derived class***. In our case, the most derived class is the `IOFile`. So `IOFile` in addition to intializing its direct parents, it also needs to initialize the ***base virtual class* - `File`**. And these two instances (`InputFile` and `OutputFile`) of initialization (`File(fname)`) are simply ignored. This is the kind of awkward and non-intuitive, but we have to live it if we want to use multiple inheritance. And this rule applies no matter how far the `IOFile` is from the `File` in the hierarchy. As long as the `IOFile` is the most derived class, it always bears the responsibility of initializing the virtual base class:
```
class InputFile : virtual public File {
    InputFile(std::string fname) : File(fname) {}
};

class OutputFile : virtual public File {
    OutputFile(std::string fname) : File(fname) {}
};

class IOFile : public InputFile, public OutputFile {
    IOFile(std::string fname) : OutputFile(fname), InputFile(fname), File() {}
};
```


### Benefits of Using Multiple Inheritance
So if the multiple inheritance is so difficult to use, why do we have to use it? The answer to that is ***interface segregation principle***. The interface segregation principle states that if an interface is too large, then split the interface into smaller and most specific ones, so that for clients, will only need to know about the methods that are of interest to them. Say I want to write a program to model a person called `Andy`, and `Andy` is a very complicated person, he can do a lot of things, he can provide a lot of services. So to completely model `Andy`, I might end up having a class that has ***500 APIs***. This will make `Andy` very difficult person to deal with. He has 500 APIs, how am I supposed to talk to him? It turns out it doesn't have to be that difficult. `Andy` is also an `Engineer`. The `Engineer` class provide all the services that the `Engineer` can provide, which is a much smaller number of ***API***, says ***40 APIs***. `Andy` is an `Engineer`, so we can publicly derived from `Engineer`:
```
class Engineer {
    ...40 APIs...
};

class Son {
    ...50 APIs...
};

...

class Andy : public Engineer, Son {
public:
    ...500 APIs...
};
```
And I'm a co-worker for `Andy`, so in order to have a happy-time co-working with `Andy`, I only need to know about `Andy`'s `Enginner` APIs. I don't necessarily need to know about any other things that `Andy` does. In other works, I only want to instantiate `Andy` as an `Engineer` and talk to hime through his `Engineer` interface. However, from `Andy`'s parent's point of view, `Andy` is also a `Son`. And they want to instantiate `Andy` as a `Son`, and talk to him through `Son`'s interface, let's say ***50 APIs***. And `Andy` is also derived from `Son` class. And this list and go on and on as `Andy` plays different role in his life.

So as you can see, by applying ***interface segregation principle***, we are making `Andy` a much easier person to deal, because each `Andy`'s client only have have a smaller and more specific interface to know about in order to talk to `Andy`. So in our daily programming, if you end up having a `class` that has a large number of ***APIs*** and then the class is servicing different group of clients, may be it's time for you to use ***interface segregation principle***. And when using ***interface segregation principle***, the ***multiple inheritance*** has a essential role to play.


### Pure Abstract Classes
Now you understand the important of ***multiple inheritance***. The question is how can we use multiple inheritance without getting into all the troubles that we've talked about in previous examples? To answer that question, let me introduce a new concept, it's called ***pure abstract classes***.

C++ standard provides the concept of ***abstract class***. And ***abstract class*** is a class that has one or more ***pure virtual methods***.

A ***pure abstract class*** is a class that contains only ***pure virtual methods***. It has no data, no concrete methods. And here is an example of ***pure abtract classes***:
```
class OutputFile {
public:
    void write() = 0;
    void open() = 0;
};
```
In a nutshell, a ***pure abstract class*** is **a class that has no incrementation**, **it only provides interface and it provides zero implementation**. And it turns out all the problems that we've talked about, duplication of data, duplication of the methods, the initialization problems. All the problems will be gone, if we only derived from ***pure abstract classes***. If all the base classes `InputFile`, `OutputFile` and `File` are ***pure abstract classes*** that has no implementation. Then there is no such problem as duplicated implementation and initialization, because there is nothing to duplicate and nothing to initialize anyway. And we don't even need the virtual inheritance anymore.

So all the problems are gone, but we still have the benefits of ***interface segregation principle***, because the ***interface segregation principle*** cares nothing about the implementation. It only cares about the interface. It will work out perfectly fine if `Engineer` and `Son` are ***pure abstract classes***.


### Summary
1. ***Multiple inheritance*** is an important technique to have. One example is ***interface segregation principle* - *ISP***.
   
2. If you are using multiple inheritance, it is strongly recommended that you only derived from ***pure abstract classes***.