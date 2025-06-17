1) A counting_semaphore is a lightweight synchronization primitive that can control access to a shared resource. Unlike a std::mutex, a counting_semaphore allows more than one concurrent access to the same resource, for at least LeastMaxValue concurrent accessors. The program is ill-formed if LeastMaxValue is negative.
2) binary_semaphore is an alias for specialization of std::counting_semaphore with LeastMaxValue being 1. Implementations may implement binary_semaphore more efficiently than the default implementation of std::counting_semaphore.
A counting_semaphore contains an internal counter initialized by the constructor. This counter is decremented by calls to acquire() and related methods, and is incremented by calls to release(). When the counter is zero, acquire() blocks until the counter is incremented, but try_acquire() does not block; try_acquire_for() and try_acquire_until() block until the counter is incremented or a timeout is reached.

Similar to std::condition_variable::wait(), counting_semaphore's try_acquire() can spuriously fail.

Specializations of std::counting_semaphore are not DefaultConstructible, CopyConstructible, MoveConstructible, CopyAssignable, or MoveAssignable.

Data Members
Member name	Definition
counter (private)	The internal counter of type std::ptrdiff_t.
(exposition-only member object*)
Member functions
(constructor)
 
constructs a counting_semaphore
(public member function)
(destructor)
 
destructs the counting_semaphore
(public member function)
operator=
  
[deleted]
 
counting_semaphore is not assignable
(public member function)
Operations
release
 
increments the internal counter and unblocks acquirers
(public member function)
acquire
 
decrements the internal counter or blocks until it can
(public member function)
try_acquire
 
tries to decrement the internal counter without blocking
(public member function)
try_acquire_for
 
tries to decrement the internal counter, blocking for up to a duration time
(public member function)
try_acquire_until
 
tries to decrement the internal counter, blocking until a point in time
(public member function)
Constants
max
  
[static]
 
returns the maximum possible value of the internal counter
(public static member function)
Notes
As its name indicates, the LeastMaxValue is the minimum max value, not the actual max value. Thus max() can yield a number larger than LeastMaxValue.

Unlike std::mutex a counting_semaphore is not tied to threads of execution - acquiring a semaphore can occur on a different thread than releasing the semaphore, for example. All operations on counting_semaphore can be performed concurrently and without any relation to specific threads of execution, with the exception of the destructor which cannot be performed concurrently but can be performed on a different thread.

Semaphores are also often used for the semantics of signaling/notifying rather than mutual exclusion, by initializing the semaphore with ​0​ and thus blocking the receiver(s) that try to acquire(), until the notifier "signals" by invoking release(n). In this respect semaphores can be considered alternatives to std::condition_variables, often with better performance.