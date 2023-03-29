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
We have talked about how to use mutex to synchronize the access of common resource among the thread. Now you're going to talk about another synchronization issue, which using mutex alone cannot help us to solve the problem.

Let's look at an example. The `main()` function is pretty simple, it creates two threads `t1` with `function_1` and `t2` with `function_2`, and then wait for two threads to finish. First, there is a global variable `q`, which is a ***deque of integer***. And there is also a ***mutex*** `mu`. The thread `t1` has a `while`-loop, it push number into the queue, and then sleep for a second, and then go to the next loop. The thread `t2` also has a `while`-loop, it gives checking if loop is empty. If it is not empty, it pops off the data, and then prints it out. Otherwise, it go to the next loop. So thread `t1` is the producer of the data, and the thread `t2` is the consumer of the data:
```
std::deque<int> q;
std::mutex mu;

void function_1() {
    int count = 10;
    while(count > 0) {
        std::unique_lock<mutex> locker(mu);
        q.push_front(count);
        locker.unlock();
        std::this_thread::sleep_for(chrono::seconds(1));
        count--;
    }
}

void function_2() {
    int data = 0;
    while(data != 1) {
        std::unique_lock<mutex> locker(mu);
        if(!q.empty()) {
            data = q.back();
            q.pop_back();
            locker.unlock();
            std::cout << "t2 got a value from t1: " << data << std::endl;
        } else {
            locker.unlock();
        }
    }
}

int main() {
    std::thread t1(function_1);
    std::thread t2(function_2);
    t1.join();
    t2.join();
}
```
Before they go ahead and access `q`, they'll lock the mutex. This is good, because `q` is a shared memory between the thread `t1` and the thread `t2`, So if the access of `q` is not synchronized with a mutex, then there will a data race. However, there is another problem, the thread `t2` is in a busy waiting state. It keeps checking if `q` is empty, and if `q` is empty, it will unlock the lock and immediately go to the next loop. So it will keep looping.

We all know busy waiting is very inefficient. One way to improve efficiency is that if the `q` is empty, we'll let the thread to take a nap, and then go to the next loop. This will largely reduce the number of looping. But the problem is how do we decide on the time duration of its nap. If the time is too short, then the thread will still end up spending a lot of time looping. If the time is too long, then it may not be able to get the data in time. So it is very hard to find the best number, this is where the ***condition variable*** comes in.

In addition to the mutex, we also need a `std::condition_variable` object, let's say `cond`. And in thread `t1`, after we has pushed the data into `q` and unlock the `locker`, we would call `cond.notify_one`. This will wake up one thread if any that is waiting on this condition:
```
std::deque<int> q;
std::mutex mu;
std::condition_variable cond;

void function_1() {
    int count = 10;
    while(count > 0) {
        std::unique_lock<mutex> locker(mu);
        q.push_front(count);
        locker.unlock();
        cond.notify_one();          // Notify one waiting thread, if there is one
        std::this_thread::sleep_for(chrono::seconds(1));
        count--;
    }
}
```
In thread `t2`, we don't need `if`-`else`-block, we only need to call `cond.wait(locker)`, this will put thread `t2` into sleep until being notified by thread `t1`. So `std::condition_variable` can enforce that the thread `t2` will go ahead and fetch the data only after thread `t1` has pushed the data into the `q`. In other words, it can enforce certain parts of the two threads to be executed in a predefined order.
```
void function_2() {
    int data = 0;
    while(data != 1) {
        std::unique_lock<mutex> locker(mu);
        cond.wait(locker);          // spurious wake
        data = q.back();
        q.pop_back();
        locker.unlock();
        std::cout << "t2 got a value from t1: " << data << std::endl;
    }
}
```
The reason why `cond.wait()` method takes `locker` as an argument is because the mutex is locked by the thread `t2`, and a thread should never go to sleep while holding a mutex. We don't want to lock everybody out while you are sleeping. So before the `cond.wait()` method, put thread into sleep it will unlock the `locker`, and then go to sleep. And once the thread `t2` is woke up by the `cond.notify_one()` method, it will lock the `locker` again. And then continue to accessing the `q`. And after that, it will unlock the `locker`. Since we have lock and unlock the mutex many times, we have to use `std::unique_lock` for `std::condition_variable`. We cannot use `std::lock_guard`.

There is another problem. The thread `t2` can wake up by itself, and that is called ***spurious wake***. And if it is a ***spurious wake***, we don't want the thread to continue running, we want to put it back to sleep again. So the `cond.wait()` method can take another parameter, which is a predicate that determines whether the condition is really met for the thread to continue running. In this case, we will use a ***lambda function***, if `q` is not empty:
```
void function_2() {
    int data = 0;
    while(data != 1) {
        std::unique_lock<mutex> locker(mu);
        cond.wait(locker, []() { return !q.empty(); });     // if `q` is not empty
        data = q.back();
        q.pop_back();
        locker.unlock();
        std::cout << "t2 got a value from t1: " << data << std::endl;
    }
}
```
So if the thread `t2` woke up and found that the `q` is empty, it will go back to sleep again. If the `q` is not empty, it will go ahead and pop off the data.

Another thing to know is there could be more than one thread that is waiting on the same condition. If that is the case, when you call `cond.notify_one()`, it only wakes up one thread. If you want to wake up all the threads that is waiting at the same time, we should call `cond.notify_all()`. That will wake up all the threads. So with `std::condition_variable`, we can make sure that threads are running in the fixed order for certain portion of their code. In this example, the thread `t1` will push the data into `q` first, and then notify the thread `t2` to running. And then the thread `t2` will pop off the data, process the data, and go to the next loop, and waiting for the next data to be available.




# Section 07 - Future, Promise and `async()`
Let's start with a simple example. We have a function `factorial()`, which computes a factorial of `N`. In the `main()` function, we create a thread `t1` to compute factorial of `4` and print the result to `std::cout`:
```
#include <future>

void factorial(int N) {
    int res = 1;
    for(int i = N; i > 1; i--)
        res *= i;
    std::cout << "Result is: " << res << std::endl;
}

int main() {
    std::thread t1(factorial, 4);

    t1.join();
}
```
Let's say we don't just want to print the result to the `std::cout`, we want to return the result from the child thread to the parent thread. So that we can do something with it. For example, we create an integer `x`, and then pass `x` to the thread `t1` by reference. The `factorial` will take the second parameter:
```
void factorial(int N, int& x) {
    int res = 1;
    for(int i = N; i > 1; i--)
        res *= i;
    
    std::cout << "Result is: " << res << endl;
    x = res;
}

int main() {
    int x;
    std::thread t1(factorial, 4, std::ref(x));

    t1.join();
}
```
But this is not enough. `x` is a shared variable between the child thread and the parent thread, so we need to protect it with some kind of mutex. And we also want to make sure that the child thread will set the variable `x` first, and then the parent thread ahead and fetch the variable. So we may also want a `std::condition_variable`.
```
std::mutex mu;
std::condition_variable cond;

void factorial(int N, int& x) {
    int res = 1;
    for(int i = N; i > 1; i--)
        res *= i;
    
    std::cout << "Result is: " << res << std::endl;
    x = res;
}
```
Now our code becomes more complicated, we need to lock and unlock the mutex, we need to call `cond.notify()` and `cond.wait()`. And more importantly, we have two global variables `mu` and `cond` that needs to be taken care of. So our code structure become pretty messy. All we need to do is launch a thread and get the result from the thread. The standard library allow you to provide an easier way to this job. So instead of using thread object to create a thread, we're going to use `std::async()`. The thread is a class, and `std::async()` is a function. And this function returns a very important thing - `std::future` object:
```
#include <future>

int factorial(int N) { 
    int res = 1;
    for(int i = N; i > 1; i--)
        res *= i;

    std::cout << "Result is: " << res << std::endl;
    return res;
 }
 
int main() {
    int x;
    std::future<int> fu = std::async(factorial, 4);
    x = fu.get();
}
```
This future is a channel where we can get the result from the child thread. We can do `x = fu.get()`. Then the `factorial()` function doesn't need the second parameter, but it does need a return value `int`. And at the end, it needs to return `res`. And we don't need all the global variables `mu` and `cond`. So our code become much cleaner. 


The `fu.get()` method will wait until the child thread finish, and then return the value from the child thread. So conceptually, a `std::future` class represents an object where you can get something in the future. And the `std::future` object can called the get function of only once. If later on, we call the `fu.get()` again, it will crash out program. So you should not do that:
```
int main() {
    int x;
    std::future<int> fu = std::async(factorial, 4);
    x = fu.get();
    fu.get();           // crash
}
```

Let's say we're going to use `std::async()` function to create another thread, but that is not completely true. The `std::aysnc()` function may or may not create another thread. And that can be controlled by another parameter. For example, we're going to call the `std::async()` function with parameter of `std::launch::deferred`:
```
int main() {
    int x;
    std::future<int> fu = std::async(std::launch::deferred, factorial, 4);
    x = fu.get();
}
```
Now the `std::async()` function will not create the thread. It will actually defer the execution of this function until later on, when the `fu.get()` method is called. So when the `fu.get()` method is called, the `factorial()` function will be executed in the same thread.

If we launch the `std::async()` function with the `std::launch::async` parameter, then it will create another thread:
```
    std::future<int> fu = std::async(std::launch::async, factorial, 4);
```
And we also can `or` the two values together. This means that whether the `std::async()` function will create another thread or not, we will be determined by the implementation:
```
    std::future<int> fu = std::async(std::launch::async | std::launch::deferred, factorial, 4);
```
 And this `std::launch::async | std::launch::deferred` actually is a default value for this parameter, so these two line of code are exactly the same:
 ```
    std::future<int> fu = std::async(factorial, 4);
    std::future<int> fu = std::async(std::launch::async | std::launch::deferred, factorial, 4);
 ```
If you want to make sure that a new thread will be born, then we should use `std::launch::async` parameter:
```
    std::future<int> fu = std::async(std::launch::async, factorial, 4);
```

So now we have use the `std::future` to pass the child thread to the parent thread, we can also use the future to do the opposite thing. We can pass the value from the parent thread to the child thread, not at the time of creating the thread, but sometimes in the future. For that, we also need a `std::promise`. And we need another `std::future` `f`, and we will pass the future as a reference to the thread. And then this `factorial()` function will take `std::future` as a parameter, and sometimes later, it can call the `std::future` to get a value:
```
int factorial(std::future<int>& f) {     // take future as parameter
    int res = 1;

    int N = f.get();                    // call future to get value
    for(int i = N; i > 1; i--)
        res *= i;
    
    std::cout << "Result is: " << res << std::endl;
    return res;
}

int main() {
    int x;

    std::promise<int> p;
    std::future<int> f = p.get_future();

    std::future<int> fu = std::async(std::launch::async, factorial, 4, std::ref(f));

    // do something else
    p.set_value(4);

    x = fu.get();
    std::cout << "Get from child: " << x << std::endl;
}
```
By doing that, we're telling our child thread that we will send him a value, but we don't have that value yet. So we'll send it over in the future. That is our `std::promise`. At this moment, we just do whenever we can do and then wait for our package. And sometimes later, we will do something else, and may probably take a nap. And then, we will keep our promise `4`. So after we have set the value `4`, the child thread will get the value `4`. And let's print something to verify the program is good (last statement).

Let's run it. So the `Result is: 24`, and `Get from child: 24` is printed out by parent. Our program is good:
```
Result is: 24
Get from child 24
```

