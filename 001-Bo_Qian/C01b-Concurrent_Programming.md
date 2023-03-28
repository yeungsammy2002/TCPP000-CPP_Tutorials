# Section 04 - Deadlock
Let's start with the example that we have last time. We have a class `LogFile` and the `LogFile` has a mutex and an ofstream. In the `.shared_print()`, we will lock the mutex and then print things to the `ofstream`. `function_1()` counts from `0` to `-100`. And then the `main()` function create the thread of `t1` with `function_1()` and then count from `0` to `100`:
```
class LogFile {
    std::mutex _mu;
    std::ofstream _f;

public:
    LogFile() {
        _f.open("log.txt");
    }   // Need destructor to close file

    void shared_print(std::string id, int value) {
        std::lock_guard<mutex> locker(_mu);
        _f << "From " << id << ": " << value << std::endl;
    }
};

void function_1(LogFile& log) {
    for(int i = 0; i > -100; i--)
        log.shared_print("t1", i);
}

int main() {
    LogFile log;
    std::thread t1(function_1, std::ref(log));
    for(int i = 0; i < 100; i++)
        log.shared_print(string("From main: "), i);
    t1.join();
}
```

For demostration purpose let's use the `std::cout` instead of `_f`:
```
class LogFile {
    std::mutex _mu;
    std::ofstream _f;

public:
    LogFile() {
        _f.open("log.txt");
    }   // Need destructor to close file

    void shared_print(std::string id, int value) {
        std::lock_guard<mutex> locker(_mu);
        std::cout << "From " << id << ": " << value << std::endl;       // using `std::cout` instead of `_f`
    }
};

void function_1(LogFile& log) {
    for(int i = 0; i > -100; i--)
        log.shared_print("t1", i);
}

int main() {
    LogFile log;
    std::thread t1(function_1, std::ref(log));
    for(int i = 0; i < 100; i++)
        log.shared_print(string("From main: "), i);
    t1.join();
}
```

If we run the program, we will see both threads are printing things to `std::cout` in a synchronized manner:
```
...
From From main: : 47
From t1: -48
From From main: : 48
From t1: -49
From From main: : 49
From t1: -50
From From main: : 50
From t1: -51
From From main: : 51
From t1: -52
From From main: : 52
...
```

Let's say we want to protect our resource with ***two mutexes***. For example, the first one is for security reason and the second one is for file system reason:
```
class LogFile {
    std::mutex _mu;
    std::mutex _mu2;
    std::ofstream -f;
public:
    LogFile() {
        _f.open("log.txt");
    }
    void shared_print(std::string id, int value) {
        std::lock_guard<mutex> locker(_mu);
        std::lock_guard<mutex> locker2(_mu2);
        std::cout << "From " << id << ": " << value << std::endl;
    }
    void shared_print2(std::string id, int value) {
        std::lock_guard<mutex> locker2(_mu2);
        std::lock_guard<mutex> locker(_mu);
        std::cout << "From " << id << ": " << value << std::endl;
    }
};
```
To protect our resource, we need ***two lockers***. First one for `_mu` is called `locker` and second one for `_mu2` is called `locker2`. We also need ***two shared print***, `shared_print()` and `shared_print2()`. `shared_print2()` will lock `_mu2` first and then lock `_mu`. And `function_1` will call the original `shared_print()`, and the main thread will call `shared_print2()`.

Now, if we run the program again, the program didn't finished. It stopped in the middle and hang on there:
```
...
From From main: : 14
From t1: -14
From From main: : 15
From t1: -15
From From main: : 16
From t1: -16
From From main: : 17
```
What had happened is the ***classic deadlock situation***. The `t1` thread locks the mutex `_mu`, and before `t1` go ahead and lock `_mu2`, the main thread locks the mutex `_mu2`. So the `t1` is waiting for the main thread release `_mu2` and the main thread is waiting for `t1` to release `_mu`. It is ***deadlock***.

To avoid ***deadlock***, you need to make sure everybody is locking to mutexes in the same order. So in the `shared_print2()`, instead of `_mu2` first and then `_mu`, we can do in the same order as `shared_print()`:
```
    void shared_print(std::string id, int value) {
        std::lock_guard<mutex> locker(_mu);
        std::lock_guard<mutex> locker2(_mu2);
        std::cout << "From " << id << ": " << value << std::endl;
    }
    void shared_print2(std::string id, int value) {
        std::lock_guard<mutex> locker(_mu);
        std::lock_guard<mutex> locker2(_mu2);
        std::cout << "From " << id << ": " << value << std::endl;
    }
```
Now if we run the program again, the program finished without a deadlock:
```
...
From From main: : 96
From t1: -96
From From main: : 97
From t1: -97
From From main: : 98
From t1: -98
From From main: : 99
From t1: -99
```


### `std::lock` & `std::adopt_lock`
C++ library actually provides a better solution. It's called `std::lock()` function, which can be used to lock arbitrary number of lockable objects such as mutex, using certain deadlock avoidance algorithm, so we can lock `_mu` and `_mu2`. And when we create the locker, we need another parameter called `std::adopt_lock`, it tells the locker that the mutex is already locked, all you need to do is to adopt the ownership of the mutex. So that when you go out of scope, remember to unlock the mutex:
```
void shared_print(std::string id, int value) {
    std::lock(_mu, _mu2);
    std::lock_guard<mutex> lock(_mu, std::adopt_lock);
    std::lock_guard<mutex> lock(_mu2, std::adopt_lock);
    std::cout << "From " << id << ": " << value << std::endl;
}
void shared_print2(std::string id, int value) {
    std::lock(_mu, _mu2);
    std::lock_guard<mutex> lock(_mu, std::adopt_lock);
    std::lock_guard<mutex> lock(_mu2, std::adopt_lock);
    std::cout << "From " << id << ": " << value << std::endl;
}
```
Now if we run the program again, then the program also finished without any deadlock:
```
...
From From main: : 97
From t1: -94
From From main: : 98
From t1: -95
From From main: : 99
From t1: -96
From t1: -97
From t1: -98
From t1: -99
```


- ### Avoiding Deadlock - 1. Prefer Locking Single Mutex at a time
Let's analyze what you can do to avoid deadlock. First of all, you need to evaluate the situation, do you really need to lock two or more mutexes at the same time. Sometimes you don't, if that is the case, you need to one mutex at the time. We lock the mutex `_mu` and then do a bunch of different things. And then we lock the mutex `_mu2` and do other things. 
```
void shared_print2(std::string id, int value) {
    {
        std::lock_guard<mutex> locker(_mu);
        ...
    }
    {
        std::lock_guard<mutex> locker2(_mu2);
        std::cout << "From " << id << ": " << value << std::endl;
    }
}
```
So that's the first thing to avoid deadlock: 


- ### Avoiding Deadlock - 2. Avoid Locking a Mutex and then Calling a User Provided Function
The second thing is try not to lock the mutex and then call some users function. Because then we'll never know what the user provided function will do, it might end up locking another mutex and then you have two mutex being locked:
```
void shared_print2(std::string id, int value) {
    {
        std::lock_guard<mutex> locker(_mu);
        usr_function();
        ...
    }
    {
        std::lock_guard<mutex> locker2(_mu2);
        std::cout << "From " << id << ": " << value << std::endl;
    }
}
```


- ### Avoiding Deadlock - 3. Use `std::lock()` to Lock more than One Mutex
If you really want to lock more than two mutexes at a time, then try to use the `std::lock()` function to lock them, because the `std::lock()` function provide some deadlock avoidance algorithm to lock the mutex. 


- ### Avoiding Deadlock - 4. Lock the Mutex in Same Order
Wometimes, using the `std::lock()` function is not possible, then you can try to lock the mutex in same order for all threads. Or you can provide the hierarchy of mutex, so that when a thread is holder a lower level of mutex, it is not allowed to lock a higher level of mutex.


### Locking Granularity
Generally speaking, the locking of resource should happen at an appropriate granularity. A ***fine-grained lock*** protects a small amount of data. A ***coarse-grained lock*** protects a big amount of data.

If your locks are too fine-grained, then your programs will becomes tricky, and then you're more exposed to deadlocks. If your locks are too coarse-grained, then you're losing a big opportunity of parallel computing, because many threads will spend a lot of time waiting for the resources.




# Section 05 - Unique Lock and Lazy Initialization
***Unique lock*** is a different lock than the ***lock guard***. We'll also talk about how to handle lazy initialization in the threading environment.

Let's start with the example that we have in the last time. We have a class `LogFile`, inside `LogFile`, we have a ***mutex*** `_mu`, which is to synchronize the access of `std::ofstream`. Inside of the `shard_print()`, before we're writing anything to the `_f`, we use the `std::lock_guard` to lock the mutex. Using `std::lock_guard` is one way of locking the mutex. 
```
class LogFile {
    std::mutex _mu;
    std::ofstream _f;
public:
    LogFile() {
        _f.open("log.txt");
    }   // Need destructor to close file
    void shared_print(std::string id, int value) {
        std::lock_guard<mutex> locker(_mu);
        _f << "From " << id << ": " << value << std::endl;
    }
}
```
Another way is calling the mutex's own lock and unlock function, which is not recommended.

There is a third way to lock up mutex, which is using `std::unqiue_lock`:
```
class LogFile {
    std::mutex _mu;
    std::ofstream _f;
public:
    LogFile() {
        _f.open("log.txt");
    }   // Need destructor to close file
    void shared_print(std::string id, int value) {
        std::unique_lock<mutex> locker(_mu);
        _f << "From " << id << ": " << value << std::endl;      // synchronized with the mutex
        locker.unlock();
    }
}
```
`std::unique_lock` is similar to `std::lock_guard`, but it provides more flexibility. Say for example after printing to `std::ofstream`, there are a bunch of other things that need to be done, but that doesn't require the mutex to be locked. Then with `std::unique_lock`, we can actually unlock the mutex (`locker.unlock()`), so that only this printing is synchronized with the mutex.

So `std::unique_lock` provides a more flexible way to implement a finer-grained lock. With the `std::unique_lock`, you can even construct the locker without actually locking the mutex. To do that, you need to pass over another parameter `std::defer_lock`. Now the locker is the owner of the mutex but the mutex is not locked yet.
```
class LogFile {
    std::mutex _mu;
    std::ofstream _f;
public:
    LogFile() {
        _f.open("log.txt");
    }   // Need destructor to close file
    void shared_print(std::string id, int value) {
        std::unique_lock<mutex> locker(_mu, std::defer_lock);
        // do something else

        _f << "From " << id << ": " << value << std::endl;
        locker.unlock();
        ...

        locker.lock();
    }
}
```
Now you have the opportunity to do something else that doesn't access the `std::ofstream`, therefore doesn't need to lock the mutex. And before you start to write things into the `std::ofstream`, you can lock the mutex. And sometimes later, if you want to lock the mutex again, you can do that again `locker.lock()`. So you can lock and unlock aperture any number of times in the code, which you cannot do with `std::lock_guard`.

Note that a wrapper class of a mutex, whether it's `std::lock_guard` or `std::unique_lock` can never be copied. However, a `std::unique_lock` can be moved when you move a `std::unique_lock`. You are transferring the ownership of a mutex from one `std::unique_lock` to another `std::unique_lock`.

So if we create another `std::unique_lock` `locker2`, we can use a `std::move()` function to move the ownership of the mutex from `locker` to `locker2`. And a `locker` can never be moved:
```
    std::unique_lock<mutex> locker2 = std::move(locker);
```
So these are the flexibilities that a `std::unqiue_lock` can provided. But the flexibility of a `std::unique_lock` is not free. The `std::unique_lock` is a little bit more heavy weighted than a `std::lock_guard`. So if the performance is what we would really concern about and you don't really need the extra flexibility of the `std::unique_lock`, you might just use the `std::lock_guard`.


In this example, we have opened the log file in the constructor, but sometimes you don't want that. For example, if it turns out that the `shared_print()` has never been called, then we have opened the log file for nothing. Say we want to make sure the file will be open only if the `shared_print()` is called. Instead of open the file in the constructor, we're going to open it in the `shared_print()` function. And if the file is not open, it will open the file, so the file will be open only once in the `shared_print()` function. This is known as ***lazy initialization*** or ***initialization upon first use idiom***. Then we need to make sure the program is thread safe. To do that, we need to use mutex to synchronize the file opening process. We don't want to reuse `_mu` because this mutex is used to print messages, and print message could happen many many times but the file only need to be open once. So we will use a different mutex called `_mu_open`. Before we open a file, we will use `std::unique_lock`. Now, before a thread can open the lock file, it has to lock the mutex. 
```
class LogFile {
    std::mutex _mu;
    std::mutex _mu_open;
    ofstream _f;
public:
    LogFile() {
    }   // Need destructor to close file
    void shared_print(std::string id, int value) {
        if(!_f.is_open()) {
            std::unique_lock<mutex> locker2(_mu_open);
            _f.open("log.txt");
        }

        std::unique_lock<mutex> locker(_mu, std::defer_lock);
        _f << "From " << id << ": " << value << std::endl;
    }
}
```
But the program is still not thread safe. Let's say we have two threads running: ***thread A*** and ***thread B***. First, ***thread A*** comes here `if(!_f.is_open()) {...}` and found that file is not open. So they're go ahead and lock mutex, and start opening file. Before it open the file, ***thread B*** also comes here `if(!_f.is_open()) {...}` and found that file is not open yet. So it also try to lock the mutex, and of course being blocked over here `std::unique_lock<mutex> locker2(_mu_open);`. Once ***thread A*** has opened at the file and exited the `if`-block, ***thread B*** will get the mutex and ***thread B*** will open the file again. So the file will be opened twice by two threads. So not only the `.open()` method needed to be synchronized, the `.is_open()` function also needs to be synchronized. So we will protect both operation with the same mutex:
```
class LogFile {
    std::mutex _mu;
    std::mutex _mu_open;
    ofstream _f;
public:
    LogFile() {
    }   // Need destructor to close file
    void shared_print(std::string id, int value) {
        {
            std::unique_lock<mutex> locker2(_mu_open);
            if(!_f.is_open()) {
                _f.open("log.txt");
            }
        }

        std::unique_lock<mutex> locker(_mu, std::defer_lock);
        _f << "From " << id << ": " << value << std::endl;
    }
}
```
Now this program is thread-safe. But it introduced another problem. The file only needs to be opened once but now every we call the `shared_print()` method, the program will lock the mutex, check if the file is opened, and then unlock the mutex, so all these locking and unlocking are purely wasting of computer cycles. And more importantly, those extra useless locking of mutex hinders the program from being run concurrently, which is bad.

The standard library provides a solution specifically for this kind of problem. Instead of using another mutex, we're going to use `std::once_flag`, and don't need all this locking and file checking (`std::unique_lock` & `if(!_f.is_open())`). Instead, we only need to call this function `std::call_once()`, and with `_flag` and the lambda function, which opens the file. This will make sure this lambda function will be called only once, and only by one thread.
```
class LogFile {
    std::mutex _mu;
    std::once_flag _flag;
    std::ofstream _f;
public:
    LogFile() {
    }
    void shared_print(std::string id, int value) {
        std::call_once(_flag, [&]() { _f.open("log.txt"); });   // file will be opened only once by 

        std::unique_lock<mutex> locker(_mu, std::defer_lock);
    }
}
```
So comparing to what we have done before, our life becomes much easier. And our program is both efficient and accurate.




# Section 06 - Condition Variables

