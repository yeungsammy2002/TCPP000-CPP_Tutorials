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

To solve this problem, we can use, again, a `clone()` method. We define a public virtual `clone()` method in `Dog` class, and this method will return a `Dog` object, which is copy constructed from itself. And then the `YellowDog` class also have a `clone()` method, but it will return a `YellowDog` object instead of `Dog`. And in the `foo()` function, instead of calling the copy constructor directly, we will the `clone()` method. And now, `c` is indeed a `YellowDog` object. So in a sense, the `clone` method is like a vitrual constructor. It make sure the appropriate type of object will be constructed no matter what kind of type the original object has been casted into.
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

# 14 - 3:11
