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

There are still one minor problem and one major problem. The minor problem is since this `t` is no longer used in the main thread, we can move it to the `task_q`:
```
int factorial(int N) {
    int res = 1;
    for(int i = N; i > 1; i--)
        res *= i;
    std::cout << "Result is " << res << std::endl;
    return res;
}

std::deque<std::packaged_task<int()>> task_q;
std::mutex mu;
std::condition_variable cond;

void thread_1() {
    std::packaged_task<int()> t;
    {
        std::unique_lock<std::mutex> locker(mu);        // using unique lock instead of lock guard
        cond.wait(locker, [](){ !task_q.empty() });
        t = std::mutex
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
        task_q.push_back(std::move(t));             // move
    }
    cond.notify_one();

    std::cout << fu.get();

    t1.join();
}
```
And then the major problem is the `thread_1` might call the `.front()` method before the main thread call the `q.push_back()` method. That will become a disaster. So we need to make sure the `.front()` method is called after the `q.push_back()` method. To do that, we need a `std::condition_variable`. And before calling the `.front()` method, we will call `cond.wait()` with a predicate. This predicate verified that the `task_q` is not empty. And to use condition variable, we should not use `std::lock_guard`, we need to use `std::unique_lock`. And the same thing in the main thread, where you call `cond.notify_one()`. Now this program is thread safe.

So this is how to use a `std::packaged_task` for threading. We have also giving you a review of how to use a `std::mutex` and `std::condition_variable`.



## Summary of Different Ways of Getting a Future
There are 3 ways of getting a future:
1. `std::promise` has a method `std::promise::get_future()`
2. `std::packaged_task` has a method `std::packaged_task::get_future()`
3. `std::async()` function returns a future




# Section 10 - Review & Time Constrains
Today we'll give you a review of the things that we have learned and also show you how to add the time constrains to your threads.



Let's reuse our `factorial()` function. 
```
int factorial(int N) { ... }

int main() {
    /* Thread */
    std::thread t1(factorial, 6);

    /* Mutex */
    std::mutex mu;
    std::lock_guard<std::mutex> locker(mu);
    std::unique_lock<std::mutex> ulocker(mu);

    /* Condition Variable */
    std::condition_variable cond;

    /* Promise and Future */
    std::promise<int> p;
    std::future<int> f = p.get_future();

    /* async() */
    std::future<int> fu = async(factorial, 6);

    /* Packaged Task */
    std::packaged_task<int(int)> t(factorial);
    std::future<int> fu2 = t.get_future();
    t(6);
}
```
- #### *Thread* - We have learned ***thread***. How to create a thread object and spawn a thread (`t1`), so it create `t1` with a `factorial()` function.

- #### *Mutex* - We also learned the ***mutex*** to sychronize the data access. And the mutex has a member method `.lock()` and `unlock()`, but they are not recommended to use directly. Instead, we should use `std::lock_guard`. If you need an extra flexiblilty, you should use `std::unique_lock`. And the `std::unique_lock` can lock and unlock a mutex multiple times, and it also can transfer the ownership of a mutex from one unique lock to another.

- #### *Condition variable* - We have learned *condition variable*. Condition variable is to synchronize the execution order of threads.

- #### *Promise* & *Future* - We have learned *promise* and *future*. `p` is promise to send an integer variable. We can create the future `f`. And `f` can be used to fetch the variable send over by `p`.

- #### `async()` - We have learned `async()` function, which is another way to spawn a thread. And `async()` function can also return a *future*. `async()` can either spawn a child thread to launch the `factorial()` function, or run the `factorial()` function in the same thread.

- #### *Packaged Task* - Finally, we have learned *packaged task*. Packaged task is a class template that can be parameterized the ways the function signature of the task we're going to create. In this case, `factorial()`. And later on, the task can be executed the same way you run the function. And the task can also return the future `fu2` to fetch the return the value from the task.

So these are the threading utilities that the standard library provides. And we can add a time constraint to some of the utility functions by using the `<chrono>` library (you can find them in the Modern C++ playlist).

Let's start with the thread. Any thread can sleep for a certain amount of time:
```
int main() {
    std::thread t1(factorial, 6);
    std::this_thread::sleep_for(std::chrono::milliseconds(3));
}
```
So this thread will sleep for 3 milliseconds.

Now let's define a time point. We have a time point `tp`, which is of the steady clock and it is equal to the current time of steady clock plus 4 microseconds. And then we can also do `std::this_thread::sleep_until` the time of `tp`. Mutex also have similar thing, we can call the `ulocker.try_lock()`, this will try to lock the mutex. And if it's not successful, it will immediately return. And `ulocker` can also try lock for a certain amount of time, in this case, if `500` has passed and then the mutex still cannot be locked, then the function will return. We can also do `ulocker.try_lock_until()` with a certain time point `tp`.
```
int main() {
    ...
    std::chrono::steady_clock::time_point tp = std::chrono::steady_clock::now() + std::chrono::microseconds(4);
    std::this_thread::sleep_until(tp);
    ...
    ulocker.try_lock();
    ulocker.try_lock_for(std::chrono::nanoseconds(500));
    ulocker.try_lock_until(tp);
}
```
You probably have seen the pattern, the functions that end with `_for()` takes duration for parameter. And in the functions ends with `_until()` takes time point for parameter, and same thing for ***condition variable***. We have used the `.wait()` method, and there's a `.wait_for()`, which takes a duration. And `cond.wait_until()` takes a time point `tp`:
```
    std::condition_variable cond;
    cond.wait_for(std::chrono::microseconds(2));
    cond.wait_until(tp);
```
