use for compile g++ -std=c++20 main.cpp -pthread -o main
Callable Functions use()

### Thread Requires a Callable

* When you create a `std::thread`, the **first argument must be a callable**:

  * Function
  * Lambda
  * Functor (i.e., a class or struct with `operator()`)
  * `std::bind` result
  * Pointer to member function (with object)

```cpp
std::thread t(some_callable, args...);
```

---

### 🔹 2. Using a Struct or Class with `std::thread`

If you want to pass a **struct or class** to a thread, you must make it **callable**. This means:

✅ You must define `operator()` inside the struct/class.

#### ✅ Example:

```cpp
struct MyTask {
    void operator()() const {
        std::cout << "Running in thread.\n";
    }
};

// Usage:
std::thread t(MyTask());  // Or std::thread t = std::thread(MyTask());
```

---

### 🔹 3. Why Must It Be Callable?

* `std::thread` needs a function-like object to invoke.
* If the passed object is **not callable**, you'll get a compilation error like:

  ```
  no matching function for call to ...
  ```

---

### 🔹 4. Summary

| Scenario        | Requirement                                                    |
| --------------- | -------------------------------------------------------------- |
| Free function   | Just pass the function                                         |
| Lambda function | Pass directly                                                  |
| Struct/class    | Must define `operator()`                                       |
| Member function | Use `std::bind` or pass pointer to member function with object |

---

### 🔸 Example with Constructor and `operator()`

```cpp
template<typename T>
struct Task {
    T data;
    Task(T val) : data(val) {}

    void operator()() const {
        std::cout << "Data: " << data << "\n";
    }
};

// Used like this:
std::thread t(Task<int>(10));
```

Let me know if you'd like this formatted as slides, comments in code, or Markdown for documentation.


The Key Parts:
std::thread t1(threadFunction, 42);: Constructs a thread and starts executing the function.

joinable(): Checks if thread is running or hasn't been joined/detached yet.

get_id(): Gets unique ID of the thread.

native_handle(): Returns native handle (platform-specific).

hardware_concurrency(): Suggests number of threads that can run concurrently.

join(): Waits for the thread to complete.

detach(): Detaches the thread to run independently.

swap(): Swaps thread ownership.

operator= via std::move: Transfers ownership of thread.

std::swap: Non-member version of thread swapping.