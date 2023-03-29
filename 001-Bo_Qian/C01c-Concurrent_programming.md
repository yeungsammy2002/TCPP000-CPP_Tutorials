# Section 08 - Using Callable Objects
We have a class `A`, and `A` has a member method `f`, which takes an `int` and `char` as parameters. And another method `g` takes a `double`. And `operator()`, which indicates that `A` is also a ***functor***. In the `main()` function, we create an object of `a`. As we have discussed before, there are two ways to launch a child thread, one is using a thread object (`t1`), another one is using `std::async()` function. Both the thread constructor and the `std::async()` function have similar function signature. They take callable object, followed by a variable number of arguments. There is another function that also has a similar signature, which is `std::bind()` function, also take a callable object, followed by arguments. And there is another one `std::call_once()`, which takes `once_flag`, and then callable object and arugment.
```
class A {
public:
    void f(int x, char c) { }
    long g(double x) { return 0; }
    int operator()(int N) { return 0; }
};

int main() {
    A a;
    std::thread t1(a, 6);
    std::async(std::launch::async, a, 6);
    std::bind(a, 6);
    std::call_once(once_flag, a, 6);
}
```
As you can see, this kind of usage of using a callable object, followed by a variable number of arguments is quite popular in standard library. We will give you the review of how these can be used in different ways.

Let's use the thread as an example. So this first line of code is creating a copy of `a`, and then invoke it as a functor in a different thread:
```
    std::thread t1(a, 6);               // copy_of_a() in a different thread
```

If we don't want to make a copy since we already have `a`, we want to use the same `a` to create a different thread. Then we can create a `std::ref()` wrapper of `a` and pass it over to the child thread. This will simply launch `a` as a functor in a different thread.
```
    std::thread t2(std::ref(a), 6);     // copy_of_a() in a different thread
```

We can also create `a` in the fly. This will create temporary `A`, and then temporary `A` is moved into the thread object and then create a thread.
```
    std::thread t3(A(), 6);             // temp A;
```

This callable object doesn't have to be functor. It can be regular function, and it can also be a ***lambda function***. For example, the thread `t4` takes a lambda function, which takes an integer `x` and then return `x` times `x`. Because this is a lambda function that takes an integer parameter, so we can pass `6` to it:
```
    std::thread t4([](int x){ return x * x; }, 6);
```

To make things complete, let's define a function `foo()`. Then we can create a thread `t5`, which takes `foo` and `7` as parameters:
```
void foo(int x) {}

int main() {
    std::thread t5(foo, 7) {}
}
```

We can also create a thread with the member methods. For example, the below statement is making a copy of `a`, and then invoke `a`'s `f()` function, `8` and `'w'` as parameters in a different thread:
```
    std::thread t6(&A::f, a, 8, 'w');       // copy_of_a.f(8, 'w') in a different thread
```

If we don't want to make a copy of `a`, we can also pass over `a`'s address, so this will simply invoke `a.f()` with the parameter of `8` and `'w'`:
```
    std::thread t7(&A::f, &a, 8, 'w');      // a.f(8, 'w') in a different thread
```

If we don't want to make a copy of `a` and we don't want to pass `a` as a reference either, then we can use `std::move()` function to move `a` from the current thread to the child thread. And we need to be very careful that `a` is no longer usable in main thread:
```
    std::thread t8(std::move(a), 6);        // a is no longer usable in main thread
```

So these are the different ways of using callable object, and you can use them for `std::bind()`, `std::async()` and `std::call_once()` functions as well.




# Section 09 - Packaged Tasks
Let's use our example of `factorial()` function, which computes the factorial of the integer `N`. In the `main()` function, we create a `std::packaged_task` called `t` with `factorial` function. So `t` is a task being packaged up to a package, and then this package can be passed along to different places, such as a different function, or a different object, or a different thread. So after this, many things could happen:
```
int factorial(int N) {
    int res = 1;
    for(int i = N; i > 1; i--)
        res *= i;
    std::cout << "Result is: " << res << std::endl;
    return res;
}

int main() {
    std::packaged_task<int(int)> t(factorial);

    // ...
}
```

