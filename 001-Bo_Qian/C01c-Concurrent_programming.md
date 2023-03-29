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
Let's use our example of `factorial()` function, which computes the factorial of the integer `N`. In the `main()` function, we create a `std::packaged_task` called `t` with `factorial` function. So `t` is a task being packaged up to a package, and then this package can be passed along to different places, such as a different function, or a different object, or a different thread. So after this, many things could happen. And at the particular time of point, this task is executed, and can be executed in different context. Other than the place where it is created:
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

    t(6);       // In a different context
}
```
So this is the `std::packaged_task` mean, it is package of task that can be transported to different place in the program, and being executed over there. A package task is a template class that is parameterize with function signature of this task `factorial`. So `factorial()` takes an integer and return integer, so this is also a function that integer and return integer. And when the task is executed, it also needs to take an integer parameter. However, we cannot conveniently get the returned value from `t()`, because `t()` always return `void`. To get the return value, we have to do this `t.get_future().get()`, this will give us the return value from the `factorial()` function:
```
int main() {
    std::packaged_task<int(int)> t(factorial);

    t(6);
    int x = t.get_future().get();
}
```

Noted that `std::packaged_task` is kind of unusual. When we create a thread `t1` with `factorial` function, we can pass an additional parameter to the constructor of the thread. And this parameter will be treated as the parameter of the `factorial` function. But we cannot do that for the `std::packaged_task`, we cannot pass additional parameter into the constructor:
```
    std::thread t1(factorial, 6);
    std::packaged_task<int(int)> t(factorial, 6);       // compiler error
```
Instead, we have to use the `std::bind()` function to bind the `factorial` function with this parameter and create a function object. And this function object is then passed to the constructor of the package task to create a package task:
```
    std::packaged_task<int(int)> t(std::bind(factorial, 6));
```
Note that this new constructed function object cannot take parameter anymore, because the parameter is already bundled with the `factorial` function. So this template argument also needs to remvoe the integer parameter, and when the task is executed, it cannot take a parameter. So a packaged task is created different from the way a thread is created:
```
int main() {
    std::packaged_task<int()> t(std::bind(factorial, 6));

    // ...

    t();
}
```
It seems we can do all these things by just using the function object. Let's say `t` is just a function object, and at later point different thread, or even in a different thread, or different object, or different function, we can invoke `t`. So it seems like the function object can serve our purpose well, we don't need a `std::packaged_task`:
```
    auto t = std::bind(factorial, 6);
```
The main advantage of a `std::packaged_task` is that it can link a callable object to a future, and that is very important in a threading environment.

Now let's say we have a `task_q`, which is a deque of `std::packaged_tasks`. And in the `main()` function, we don't want to execute the task `t` in the same function, which is not very helpful. Instead, after creating the task `t`, we'll push it into the `task_q`, hoping that somebody will pop off the task and execute it in an appropriate time. And this "somebody" would be a thread, and this `thred_1` will create a `std::packaged_task` `t`. And in the `main()` function, we will create the `thread_1`, and then later on before we exit, we will call `t1.join()`. So now, the main thread will create a task, and push it into task queue. And then the `thread_1` will pop off the task from the `task_q` and execute it:
```
void thread_1() {
    std::packaged_task<int()> t;
    t = std::move(task_q.front());
    t();                            // task execute
}

int main() {
    std::thread t1(thread_1);
    std::packaged_task<int()> t(bind(factorial, 6));
    std::future<int> fu = t.get_future();
    task_q.push_back(t);

    std::cout << fu.get();
    t1.join();
}
```
Note that the `factorial` function generates a return value. So when the task `t()` is executed. It generates a returned value. We can use the task `t` to create a future `t.get_future()`. And later on, when we need the return value, we can call `fu.get()`. So this is very convenient.

The `task_q` is shared between the `thread_1` and the main thread, which means we have a **data race condition**. So we also need a mutex `mu`, and before we access the `task_q`, we need to lock the mutex, and same thing in the main thread. In the `thread_1`, we also need to pop off the task after the task is used. Note that the `front()` function under the `pop_front()` function needs to be combined with the same locker. Otherwise, the code is not thread safe:
```
std::deque<std::packaged_task<int()>> task_q;
std::mutex mu;

void thread_1() {
    std::packaged_task<int()> t;
    {
        std::lock_guard<std::mutex> locker(mu);
        t = std::move(task_q.front());
        task_q.pop_front();
    }
    t();
}

int main() {
    std::thread t1(thread_1);
    std::packaged_task<int()> t(bind(factorial, 6));
    std::future<int> fu = t.get_future();
    {
        std::lock_guard<std::mutex> locker(mu);
        task_q.push_back(t);
    }

    std::cout << fu.get();
}
```
There are one minor problem and one major problem.