The standard library provides facilities to obtain values that are returned 
and to catch exceptions that are thrown by asynchronous tasks (i.e. functions launched in separate threads).
These values are communicated in a shared state, in which the asynchronous task may write its return value or store an exception, 
and which may be examined, waited for, 
and otherwise manipulated by other threads that hold instances of std::future or std::shared_future that reference that shared state.


1) Base template.
2) Non-void specialization, used to communicate objects between threads.
3) void specialization, used to communicate stateless events.
The class template std::promise provides a facility to store a value or an exception that is later acquired asynchronously via a std::future object created by the std::promise object. Note that the std::promise object is meant to be used only once.

Each promise is associated with a shared state, which contains some state information and a result which may be not yet evaluated, evaluated to a value (possibly void) or evaluated to an exception. A promise may do three things with the shared state:

make ready: the promise stores the result or the exception in the shared state. Marks the state ready and unblocks any thread waiting on a future associated with the shared state.
release: the promise gives up its reference to the shared state. If this was the last such reference, the shared state is destroyed. Unless this was a shared state created by std::async which is not yet ready, this operation does not block.
abandon: the promise stores the exception of type std::future_error with error code std::future_errc::broken_promise, makes the shared state ready, and then releases it.
The promise is the "push" end of the promise-future communication channel: the operation that stores a value in the shared state synchronizes-with (as defined in std::memory_order) the successful return from any function that is waiting on the shared state (such as std::future::get). Concurrent access to the same shared state may conflict otherwise: for example multiple callers of std::shared_future::get must either all be read-only or provide external synchronization.


std::future_status
enum class future_status {
    ready,
    timeout,
    deferred
};

deferred	the shared state contains a deferred function, so the result will be computed only when explicitly requested
ready	the shared state is ready
timeout	the shared state did not become ready before specified timeout duration has passed

std::launch
enum class launch : /* unspecified */ {
    async =    /* unspecified */,
    deferred = /* unspecified */,
    /* implementation-defined */
};
std::launch is a BitmaskType. It specifies the launch policy for a task executed by the std::async function.

Constants
The following constants denoting individual bits are defined by the standard library:

Enumerator	Meaning
async	the task is executed on a different thread, potentially by creating and launching it first
deferred	the task is executed on the calling thread the first time its result is requested (lazy evaluation)
In addition, implementations are allowed to:

define additional bits and bitmasks to specify restrictions on task interactions applicable to a subset of launch policies, and
enable those additional bitmasks for the first (default) overload of std::async.





















# **Using `wait_for()` and `wait_until()` with `std::future`**

These functions allow you to wait for a result with timeouts, preventing indefinite blocking.

## **1. `wait_for(timeout)` - Wait with Duration**
Waits for a result, but gives up after a specified time duration.

### **Example: Waiting with Timeout**
```cpp
#include <iostream>
#include <future>
#include <chrono>

int slow_computation() {
    std::this_thread::sleep_for(std::chrono::seconds(5)); // Simulate long work
    return 42;
}

int main() {
    std::future<int> fut = std::async(std::launch::async, slow_computation);

    std::cout << "Waiting for result (max 2 seconds)...\n";

    // Wait for 2 seconds
    auto status = fut.wait_for(std::chrono::seconds(2));

    if (status == std::future_status::ready) {
        std::cout << "Result ready: " << fut.get() << "\n";
    } 
    else if (status == std::future_status::timeout) {
        std::cout << "Timeout! Task still running...\n";
    } 
    else if (status == std::future_status::deferred) {
        std::cout << "Task not yet started (deferred).\n";
    }

    return 0;
}
```
**Output:**
```
Waiting for result (max 2 seconds)...
Timeout! Task still running...
```

### **Key Points:**
- Returns `std::future_status`:
  - `ready` → Result is available.
  - `timeout` → Time expired before result was ready.
  - `deferred` → Task was deferred (if using `std::launch::deferred`).

---

## **2. `wait_until(time_point)` - Wait Until Deadline**
Waits until a specific time point (e.g., "wait until 5 seconds from now").

### **Example: Waiting Until a Deadline**
```cpp
#include <iostream>
#include <future>
#include <chrono>

int slow_computation() {
    std::this_thread::sleep_for(std::chrono::seconds(3)); // Simulate work
    return 100;
}

int main() {
    std::future<int> fut = std::async(std::launch::async, slow_computation);

    auto deadline = std::chrono::system_clock::now() + std::chrono::seconds(2);
    std::cout << "Waiting until deadline (2 seconds from now)...\n";

    auto status = fut.wait_until(deadline);

    if (status == std::future_status::ready) {
        std::cout << "Result ready: " << fut.get() << "\n";
    } 
    else if (status == std::future_status::timeout) {
        std::cout << "Deadline reached! Task still running...\n";
    }

    return 0;
}
```
**Output:**
```
Waiting until deadline (2 seconds from now)...
Deadline reached! Task still running...
```

### **Key Points:**
- Takes a `std::chrono::time_point` (absolute time).
- Useful for **real-time scheduling** (e.g., "must finish by 3:00 PM").

---

## **3. Comparison: `wait_for()` vs `wait_until()`**
| Feature | `wait_for(duration)` | `wait_until(time_point)` |
|---------|----------------------|--------------------------|
| **Argument** | Relative time (e.g., `2s`) | Absolute deadline (e.g., `now() + 2s`) |
| **Use Case** | "Wait for up to X seconds" | "Wait until a specific clock time" |
| **Return Status** | `ready`/`timeout`/`deferred` | Same as `wait_for()` |
| **Precision** | Good for short delays | Better for long-term scheduling |

---

## **4. Practical Use Cases**
1. **Task Timeout Handling**
   ```cpp
   if (fut.wait_for(2s) == std::future_status::timeout) {
       std::cout << "Task took too long, canceling...\n";
       // Handle timeout (e.g., kill the task if possible)
   }
   ```

2. **Real-time Systems**
   ```cpp
   auto deadline = std::chrono::steady_clock::now() + 500ms;
   if (fut.wait_until(deadline) == std::future_status::ready) {
       // Process result before deadline
   }
   ```

3. **Polling with Timeout**
   ```cpp
   while (fut.wait_for(100ms) != std::future_status::ready) {
       std::cout << "Still waiting...\n";
   }
   ```

---

## **5. Key Takeaways**
- **`wait_for()`** → Relative timeout ("wait up to X milliseconds").
- **`wait_until()`** → Absolute deadline ("wait until 3:00 PM").
- Both return `std::future_status` to check if the result is ready.
- Useful for **avoiding infinite waits** in async operations.

Would you like an example combining both with a **retry mechanism**? 🔄


# **Understanding `std::future` in C++**

`std::future` is a key component in C++ for handling **asynchronous operations**. It allows you to retrieve a value (or exception) that will be set in the future by another thread or task.

---

## **1. What is `std::future`?**
- A **placeholder** for a value that will be computed asynchronously.
- Provides a way to **wait** for and **retrieve** the result of an operation running in another thread.
- Works with:
  - `std::async` (launches a task)
  - `std::packaged_task` (wraps a callable object)
  - `std::promise` (manually sets a value)

---

## **2. How Does It Work?**
### **Key Concepts:**
- **Shared State**: The `future` is connected to a shared state (managed by `std::promise`, `std::async`, etc.).
- **Blocking vs. Non-blocking**:
  - `get()` blocks until the result is ready.
  - `wait_for()` / `wait_until()` allow timed waiting.
- **Single Use**: A `future` can only retrieve the result **once** (unlike `std::shared_future`).

---

## **3. Key Member Functions**
| Function | Description |
|----------|-------------|
| **`get()`** | Waits (if needed) and returns the stored value. **Can only be called once!** |
| **`valid()`** | Checks if the `future` has a shared state (`true` before `get()` is called). |
| **`wait()`** | Blocks until the result is ready. |
| **`wait_for(timeout)`** | Waits for a result, but gives up after a specified duration. |
| **`wait_until(time_point)`** | Waits until a specific time point. |
| **`share()`** | Converts the `future` into a `std::shared_future` (allowing multiple `get()` calls). |

---

## **4. Example: Using `std::future` with `std::async`**
```cpp
#include <iostream>
#include <future>
#include <thread>

int compute() {
    std::this_thread::sleep_for(std::chrono::seconds(2)); // Simulate work
    return 42;
}

int main() {
    // Launch async task and get a future
    std::future<int> fut = std::async(std::launch::async, compute);

    std::cout << "Waiting for result...\n";
    
    // Blocks until the result is ready
    int result = fut.get(); 

    std::cout << "Result: " << result << "\n";
    return 0;
}
```
**Output:**
```
Waiting for result...
(2 seconds later...)
Result: 42
```

---

## **5. Example: Using `std::future` with `std::promise`**
```cpp
#include <iostream>
#include <future>
#include <thread>

void worker(std::promise<int> prom) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    prom.set_value(100); // Send result back
}

int main() {
    std::promise<int> prom;
    std::future<int> fut = prom.get_future();

    std::thread t(worker, std::move(prom)); // Transfer ownership

    std::cout << "Waiting for promise...\n";
    int result = fut.get(); // Blocks until the promise is set

    std::cout << "Received: " << result << "\n";
    t.join();
    return 0;
}
```
**Output:**
```
Waiting for promise...
(1 second later...)
Received: 100
```

---

## **6. Common Use Cases**
1. **Thread Synchronization**: Wait for a background task to finish.
2. **Return Values from Threads**: Get results from `std::thread` (via `std::promise`).
3. **Parallel Computations**: Run multiple tasks and collect results.
4. **Timeout Handling**: Use `wait_for()` to avoid indefinite blocking.

---

## **7. Important Notes**
- ⚠️ **`get()` can only be called once** (throws `std::future_error` if reused).
- 🔄 **`std::shared_future`** allows multiple `get()` calls (shared ownership).
- 🚦 **Thread Safety**: The `future` object itself is **not thread-safe**, but the underlying shared state is.

---

## **8. Summary**
| Feature | `std::future` | `std::shared_future` |
|---------|--------------|----------------------|
| **Ownership** | Single-use | Multiple `get()` calls |
| **Thread Safety** | Not thread-safe (but shared state is) | Same as `future` |
| **Usage** | One-time result retrieval | Broadcasting results to multiple threads |

### **When to Use?**
- **`std::future`** → Single-thread result retrieval.
- **`std::shared_future`** → Multiple threads need the same result.

---

### **Final Answer**
> `std::future` is a mechanism to **retrieve asynchronous results** in C++. It works with `std::async`, `std::packaged_task`, and `std::promise` to safely pass data between threads. Key features:
> - `get()` blocks until the result is ready.
> - `wait_for()` allows timeout-based waiting.
> - **Single-use** (unlike `std::shared_future`).

Would you like an example with **exception handling** in futures? 🚀