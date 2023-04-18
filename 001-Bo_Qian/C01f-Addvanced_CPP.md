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