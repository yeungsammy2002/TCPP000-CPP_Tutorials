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
We know that virtual method is bound at ***runtime***. 

# 19 - 5:11

