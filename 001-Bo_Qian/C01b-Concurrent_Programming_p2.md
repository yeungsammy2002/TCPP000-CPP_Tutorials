# Course 01 - Concurrent Programming with C++ 11 - Part B




# Section 04 - Deadlock
Let's start with the example that we have last time. We have a class `LogFile` and the `LogFile` has a mutex and an ofstream. In the `.shared_print()`, we will lock the mutex and then print things to the `ofstream`. `function_1()` counts from `0` to `-100`. And then the `main()` function create the thread of `t1` with `function_1()` and then count from `0` to `100`:
```
class LogFile {
    std::mutex _mu;
    ofstream _f;

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
    ofstream _f;

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
    ofstream -f;
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


### Avoiding Deadlock - 1. Prefer Locking Single Mutex at a time
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


### Avoiding Deadlock - 2. Avoid Locking Mutex and then Calling User Provided Function
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

