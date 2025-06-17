# **Understanding `std::shared_future` in C++**

`std::shared_future` is an extension of `std::future` that allows **multiple threads to wait for and access the same asynchronous result**. Unlike `std::future`, which is single-use and non-sharable, `std::shared_future` can be copied and accessed by multiple threads safely.

---

## **1. Key Differences: `std::future` vs `std::shared_future`**
| Feature | `std::future` | `std::shared_future` |
|---------|--------------|----------------------|
| **Copyable?** | ❌ (Move-only) | ✅ (Can be copied) |
| **Multiple `get()` calls?** | ❌ (Single-use) | ✅ (Can be read multiple times) |
| **Thread-safe access?** | ❌ (Only one thread can `get()`) | ✅ (Multiple threads can `get()`) |
| **Use Case** | Single consumer | Multiple consumers |

---

## **2. How to Create a `std::shared_future`**
### **Method 1: Convert from `std::future`**
```cpp
std::future<int> fut = std::async(std::launch::async, []{ return 42; });
std::shared_future<int> shared_fut = fut.share(); // Transfers ownership
```
- After calling `share()`, the original `future` becomes **invalid**.

### **Method 2: Direct Construction**
```cpp
std::promise<int> prom;
std::shared_future<int> shared_fut = prom.get_future().share(); // Directly shared
```

---

## **3. Example: Multiple Threads Reading the Same Future**
```cpp
#include <iostream>
#include <future>
#include <thread>
#include <vector>

void worker(std::shared_future<int> shared_fut, int id) {
    int result = shared_fut.get(); // Blocks until ready
    std::cout << "Thread " << id << " got: " << result << "\n";
}

int main() {
    std::promise<int> prom;
    std::shared_future<int> shared_fut = prom.get_future().share();

    // Launch multiple worker threads
    std::vector<std::thread> threads;
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back(worker, shared_fut, i);
    }

    // Simulate work, then set the value
    std::this_thread::sleep_for(std::chrono::seconds(1));
    prom.set_value(100); // All threads receive this value

    for (auto& t : threads) t.join();
    return 0;
}
```
**Possible Output:**
```
Thread 2 got: 100
Thread 1 got: 100
Thread 0 got: 100
Thread 3 got: 100
Thread 4 got: 100
```
- All threads safely read the same value.

---

## **4. Key Features of `std::shared_future`**
### **1. Thread-Safe `get()`**
Multiple threads can call `get()` without race conditions:
```cpp
int val1 = shared_fut.get(); // Thread 1
int val2 = shared_fut.get(); // Thread 2 (safe)
```

### **2. Works with `wait_for()` and `wait_until()`**
Just like `std::future`, it supports timeouts:
```cpp
if (shared_fut.wait_for(1s) == std::future_status::ready) {
    int result = shared_fut.get();
}
```

### **3. Can Store Exceptions**
If the promise sets an exception, all threads will see it:
```cpp
try {
    int val = shared_fut.get();
} catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << "\n";
}
```

---

## **5. Use Cases for `std::shared_future`**
1. **Broadcasting Results**  
   - One producer sets a value, multiple consumers read it.
   - Example: Configuration loading, where many threads need the same settings.

2. **Thread Pools with Shared State**  
   - A task computes a value, and multiple workers use it.

3. **Event Notifications**  
   - A `shared_future` can act like a one-time event broadcast.

---

## **6. Pitfalls & Best Practices**
### **⚠️ Don’t Use with Frequent Updates**
- `std::shared_future` is **read-only** after the value is set.
- If you need streaming updates, consider `std::atomic` + condition variables.

### **✅ Prefer `std::shared_future` Over Global Variables**
- Safer than global data since it enforces synchronization.

### **🚫 Avoid `get()` in Performance-Critical Loops**
- `get()` may involve synchronization overhead. Cache the value if possible.

---

## **7. Summary**
- **`std::shared_future`** = Sharable, multi-thread-safe version of `std::future`.
- **Use when** multiple threads need the same async result.
- **Avoid when** you need frequent updates or mutable state.

Would you like an example combining `std::shared_future` with `std::async`? 🚀