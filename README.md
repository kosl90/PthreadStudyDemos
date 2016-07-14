# PthreadStudyDemos
pthread study demos for https://computing.llnl.gov/tutorials/pthreads/

# Find pthread in CMake

For short, using `FindThreads` module. `find_package(Threads REQUIRED)` and `${CMAKE_THREAD_LIBS_INIT}` for linking.

In cmake, `find_package` can be used to search two forms of packages, Config-file packages and Find-module packages.
Config-file is provided by upstreams, a Foo lib may provide such a config file:
> <prefix>/lib/cmake/foo-1.2/FooConfig.cmake

> <prefix>/lib/cmake/foo-1.3/FooConfigVersion.cmake // optinal package version file

You can find this library by using `find_package(Foo)`. And using `CONFIG` to specify using config-file only if necessary:
`find_package(Qt4 4.8 CONFIG)`

Find-module is provided by cmake developers, each find module will define some variables to use, please check the documentation.
A "find module" is a `Modules/Find<package>.cmake` file to be loaded by the `find_package()` command when invoked for `<package>`.
Similarly using `MODULE` to only use find-module form. For instance: `find_package(Qt5 5.5 MODULE)`

Besides, `pkg-config` is supported by `FindPkgConfig` module that provides a macro named `pkg_check_modules`. Usage:
> find_package(PkgConfig REQUIRED)

> pkg_check_modules(GLIB glib-2.0)

## Useful links
* [cmake-packages](https://cmake.org/cmake/help/v3.0/manual/cmake-packages.7.html)
* [cmake-developer#find-modules](https://cmake.org/cmake/help/v3.0/manual/cmake-developer.7.html#find-modules)
* [find_package command](https://cmake.org/cmake/help/v3.0/command/find_package.html`)


# Thread creation
`pthread_create` creates a new thread and makes it executable. This routine can be called
any number of times from anywhere within your code.

The maximum number of threads that may be created by a process is implementation dependent. Programs that attempt to exceed the limit can fail or produce wrong results.

# Thread termination

* several ways in which a thread may be terminated:
  - the thread returns normally from its starting routine. Its work is done.
  - The thread makes a call to the `pthread_exit` subroutine.
  - The thread is canceled by another thread via the `pthread_cancel` routine.
  - The entire process is terminated due to making a call either the `exec()` or `exit()`
  - If main() finishes first, without calling `pthread_exit` explicitly itself.

* the `pthread_exit` routine does not close files; any files opened inside the thread will remain open after the thread is terminated.

* if main() finishes before the threads it spawned if you don't call `pthread_exit()` explicitly. All of the threads it created will terminate because main() is done and no longer exists to support the threads.

* By having main() explicitly call `pthread_exit()` as the last thing it does, main() will block and be kept alive to support the threads it created until they are done.

# Joining and detaching threads

`pthread_join(threadid, status)`

* A joining thread can math one pthread_join() call. It is a logical error to attempt multiple joins on the same thread.
* When a thread is created, one of its attributes defines whether if is joinable or detached. Only threads that are created as joinable can be joined.
* The final draft of the POSIX standard specifies that threads should be created as joinable.
  - pthread_attr_t attr;
  - pthread_attr_init(&attr);
  - pthread_setdetachestate(&attr, PTHREAD_CREATE_JOINABLE);
  - pthread_destroy(&attr);
* pthread_detach can detach a thread even through it was created as joinable.
* If a thread requires joining, consider explicitly creating it as joinable. This provides protability as not all implementation may create threads as joinable by default.
* If you known in advance that a thread will never need to join with another thread, consider creating it in a detached state. Some system resources may be able to freed.

# Utils

`pthread_self()` returns the unique, system assigned thread ID of the calling thread.

`pthread_equal` compares two thread IDs, if equal, return non-zero value, else return zero. Because thread IDs are opaque objects, the C language equivalence operator == should not be used to compare to other thread IDs or other value.

`pthread_once` executes the init exactly once in a process.

# Mutex and Condition Variable

`pthread_mutex_unlock` will error if:
  - the mutex is already unlocked
  - the mutex is owned by another thread

Condition variables provide yet another way for thread to synchronize. While mutexes implement synchronization by controlling thread access to data, condition variables allow threads to synchronize based upon the actual value of data.

without condition variables, the programmer would need to have threads continually pool, to check if the condition is met. This can be very resource consuming since the thread would be continuously busy in activity.

A condition variables is always used in conjunction with a mutex lock.

Condition Variable Usage:

In thread Main:
* declaring and initializing global data
* declaring and initializing a condition variable
* declaring and initializing an associated mutex
* create two threads A and B to work

In thread A:
* use mutex lock
* check global data(using while loop instead of if statement). Note that using while loop instead of if statement can help deal with several potential problems:
 - if several threads are waiting for the same wake up signal, they will take turns acquiring the mutex, and any one of them can modify the condition they all waited for
 - if the thread received the signal in error due to a program bug
 - The Pthreads library is permitted to issue spurious wake ups to a waiting thread without violating the standard
* call `pthread_cond_wait` to perform a blocking wait for signal from Thread-B. Note that a call to `pthread_cond_wait` automatically and atomically unlocks the associated mutex variable so that it can be used by Thread-B
* when signalled, wake up. Mutex is automatically and atomically locked
* Explicitly unlock mutex


In thread B:
* lock mutex
* change the value of global variable that Thread-A is waiting upon
* check value, if it fulfills the desired condition, signal Thread-A
* unlock mutex

# Not covered // TODO

* Thread Scheduling
* Keys: Thread-Specific data
* Mutex Protocol Attributes and Mutex Priority Management for the handling of "priority inversion" problems
* Condition Variable Sharing - across processes
* Thread Cancellation
* Threads and Signals
* Synchronization constructs - barries and locks
