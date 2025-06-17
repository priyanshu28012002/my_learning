# A Beginner's Guide to pthreads in C++

## What are pthreads?

Pthreads (POSIX threads) are a standard API for working with threads on Unix-like systems (Linux, macOS, BSD). They allow you to create and manage multiple threads of execution within a single process.

## Basic Concepts

1. **Thread**: A sequence of instructions that can run independently
2. **Main thread**: The initial thread created when your program starts
3. **Worker threads**: Additional threads you create to perform work concurrently

## Basic pthread Functions

### 1. Creating a Thread

```cpp
#include <pthread.h>
#include <iostream>

void* thread_function(void* arg) {
    std::cout << "Hello from thread!\n";
    return nullptr;
}

int main() {
    pthread_t thread_id;
    
    // Create a new thread
    int result = pthread_create(&thread_id, nullptr, thread_function, nullptr);
    
    if (result != 0) {
        std::cerr << "Error creating thread\n";
        return 1;
    }
    
    std::cout << "Hello from main thread\n";
    
    // Wait for the thread to finish
    pthread_join(thread_id, nullptr);
    
    return 0;
}
```

### 2. Passing Arguments to Threads

```cpp
void* print_number(void* arg) {
    int num = *(int*)arg;
    std::cout << "Number: " << num << "\n";
    return nullptr;
}

int main() {
    pthread_t threads[5];
    int numbers[] = {1, 2, 3, 4, 5};
    
    for (int i = 0; i < 5; i++) {
        pthread_create(&threads[i], nullptr, print_number, &numbers[i]);
    }
    
    for (int i = 0; i < 5; i++) {
        pthread_join(threads[i], nullptr);
    }
}
```

The code you've provided has a few issues to address. Let me explain the problems and show you the corrected version:

## Issues in Your Code

1. **Incorrect pthread_create usage**: The `pthread_create` function only accepts 4 parameters, but you're trying to pass 6.
2. **Type mismatch**: You're trying to pass a `std::string` and `float` to a thread function that expects a single `void*` argument.
3. **Passing local variables by reference**: The variables `neel` and `ka` are local to `main()` and may go out of scope.

## Corrected Version

Here's how to properly pass multiple arguments to a thread function:

### Method 1: Using a Struct (Recommended)

```cpp
#include <iostream>
#include <string>
#include <pthread.h>

// Define a struct to hold all arguments
struct ThreadArgs {
    int num;
    float neel;
    std::string ka;
};

void* print_number(void* arg) {
    ThreadArgs* args = static_cast<ThreadArgs*>(arg);
    std::cout << "Number: " << args->num 
              << ", Float: " << args->neel
              << ", String: " << args->ka << "\n";
    delete args; // Clean up allocated memory
    return nullptr;
}

int main() {
    pthread_t threads[5];
    int numbers[] = {1, 2, 3, 4, 5};
    
    for (int i = 0; i < 5; i++) {
        // Create argument struct on heap
        ThreadArgs* args = new ThreadArgs{numbers[i], 2.2f, "ka"};
        
        pthread_create(&threads[i], nullptr, print_number, args);
    }
    
    for (int i = 0; i < 5; i++) {
        pthread_join(threads[i], nullptr);
    }
    
    return 0;
}
```

### Method 2: Using Multiple Arguments (Alternative)

If you want to avoid structs, you can pack multiple values into a single pointer:

```cpp
#include <iostream>
#include <string>
#include <pthread.h>
#include <tuple>

void* print_number(void* arg) {
    auto args = static_cast<std::tuple<int, float, std::string>*>(arg);
    std::cout << "Number: " << std::get<0>(*args)
              << ", Float: " << std::get<1>(*args)
              << ", String: " << std::get<2>(*args) << "\n";
    delete args;
    return nullptr;
}

int main() {
    pthread_t threads[5];
    int numbers[] = {1, 2, 3, 4, 5};
    
    for (int i = 0; i < 5; i++) {
        auto args = new std::tuple<int, float, std::string>(numbers[i], 2.2f, "ka");
        pthread_create(&threads[i], nullptr, print_number, args);
    }
    
    for (int i = 0; i < 5; i++) {
        pthread_join(threads[i], nullptr);
    }
    
    return 0;
}
```

## Key Points

1. **Single Argument**: Thread functions can only accept one `void*` argument
2. **Memory Management**: Allocate arguments on heap when passing to threads
3. **Type Safety**: Use proper casting when retrieving arguments
4. **Cleanup**: Always free allocated memory to prevent leaks

## Compilation

Remember to compile with pthread support:
```bash
g++ -std=c++11 -pthread your_program.cpp -o your_program
```



### 3. Returning Values from Threads

```cpp
void* compute_square(void* arg) {
    int num = *(int*)arg;
    int* result = new int(num * num);
    return result;
}

int main() {
    pthread_t thread;
    int number = 5;
    
    pthread_create(&thread, nullptr, compute_square, &number);
    
    int* result;
    pthread_join(thread, (void**)&result);
    
    std::cout << "Square: " << *result << "\n";
    delete result;
}
```

## Thread Synchronization

### 1. Mutexes (Mutual Exclusion)

```cpp
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int shared_counter = 0;

void* increment_counter(void* arg) {
    for (int i = 0; i < 100000; i++) {
        pthread_mutex_lock(&mutex);
        shared_counter++;
        pthread_mutex_unlock(&mutex);
    }
    return nullptr;
}

int main() {
    pthread_t t1, t2;
    
    pthread_create(&t1, nullptr, increment_counter, nullptr);
    pthread_create(&t2, nullptr, increment_counter, nullptr);
    
    pthread_join(t1, nullptr);
    pthread_join(t2, nullptr);
    
    std::cout << "Final counter: " << shared_counter << "\n";
    pthread_mutex_destroy(&mutex);
}
```

### 2. Condition Variables

```cpp
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
bool ready = false;

void* producer(void* arg) {
    pthread_mutex_lock(&mutex);
    std::cout << "Producer preparing data...\n";
    sleep(2); // Simulate work
    ready = true;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
    return nullptr;
}

void* consumer(void* arg) {
    pthread_mutex_lock(&mutex);
    while (!ready) {
        pthread_cond_wait(&cond, &mutex);
    }
    std::cout << "Consumer got data!\n";
    pthread_mutex_unlock(&mutex);
    return nullptr;
}

int main() {
    pthread_t prod, cons;
    
    pthread_create(&prod, nullptr, producer, nullptr);
    pthread_create(&cons, nullptr, consumer, nullptr);
    
    pthread_join(prod, nullptr);
    pthread_join(cons, nullptr);
    
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
}
```

## Common Pitfalls

1. **Race Conditions**: When threads access shared data without synchronization
2. **Deadlocks**: When threads wait for each other indefinitely
3. **Memory Leaks**: Forgetting to free memory returned from threads

## Best Practices

1. Always check return values from pthread functions
2. Use mutexes to protect shared data
3. Consider using higher-level abstractions (like C++'s `<thread>`) when possible
4. Keep critical sections (locked code) as short as possible

## Exercise

Create a program that:
1. Spawns 3 threads
2. Each thread computes the sum of numbers from 1 to N (where N is different for each thread)
3. The main thread collects and prints all results

```cpp
// Try implementing this exercise!
```

Remember to compile pthread programs with `-pthread` flag:
```bash
g++ program.cpp -pthread -o program
```

# Advanced Thread Control with pthreads

Pthreads provides low-level control over threads at the system level. Here's how to access and control various thread attributes:

## 1. Setting Thread Affinity (Core Binding)

```cpp
#include <pthread.h>
#include <sched.h>

void set_thread_affinity(pthread_t thread, int core_id) {
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(core_id, &cpuset);
    
    if (pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset) != 0) {
        perror("pthread_setaffinity_np");
    }
}
```

## 2. Getting Thread ID and System PID

```cpp
#include <sys/types.h>
#include <unistd.h>

void print_thread_info(const char* thread_name) {
    pid_t pid = getpid();          // Process ID
    pid_t tid = gettid();          // Thread ID (Linux specific)
    pthread_t ptid = pthread_self(); // pthread ID
    
    printf("%s: PID=%d, TID=%d, pthread_t=%lu\n", 
           thread_name, pid, tid, (unsigned long)ptid);
}
```

## 3. Setting Thread Name (Linux Specific)

```cpp
#include <sys/prctl.h>

void set_thread_name(const char* name) {
    prctl(PR_SET_NAME, name, 0, 0, 0);
}
```

## 4. Setting Thread Priority and Policy

```cpp
#include <sched.h>

void set_thread_scheduling(pthread_t thread, int policy, int priority) {
    struct sched_param param;
    param.sched_priority = priority;
    
    if (pthread_setschedparam(thread, policy, &param) != 0) {
        perror("pthread_setschedparam");
    }
}
```

## 5. Complete Example with All Controls

```cpp
#include <iostream>
#include <pthread.h>
#include <sched.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/prctl.h>

struct ThreadConfig {
    int core_id;
    int priority;
    const char* name;
};

void* thread_work(void* arg) {
    ThreadConfig* config = static_cast<ThreadConfig*>(arg);
    
    // Set name
    prctl(PR_SET_NAME, config->name, 0, 0, 0);
    
    // Set affinity
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(config->core_id, &cpuset);
    pthread_setaffinity_np(pthread_self(), sizeof(cpuset), &cpuset);
    
    // Set priority (FIFO policy)
    struct sched_param param;
    param.sched_priority = config->priority;
    pthread_setschedparam(pthread_self(), SCHED_FIFO, &param);
    
    // Print thread info
    std::cout << "Thread '" << config->name << "' running on core " << config->core_id
              << " with priority " << config->priority 
              << " (PID=" << getpid() << ", TID=" << gettid() << ")\n";
    
    // Do work
    for (volatile int i = 0; i < 100000000; i++) {} // Busy work
    
    return nullptr;
}

int main() {
    const int num_cores = sysconf(_SC_NPROCESSORS_ONLN);
    pthread_t threads[num_cores];
    ThreadConfig configs[num_cores];
    
    for (int i = 0; i < num_cores; i++) {
        configs[i] = {i, 80 - i*10, ("Worker-" + std::to_string(i)).c_str()};
        pthread_create(&threads[i], nullptr, thread_work, &configs[i]);
    }
    
    for (int i = 0; i < num_cores; i++) {
        pthread_join(threads[i], nullptr);
    }
    
    return 0;
}
```

## 6. Checking Thread Information from Shell

While your program runs, you can check thread details from another terminal:

```bash
# List all threads with their CPU affinity
ps -eLo pid,tid,psr,cmd | grep your_program_name

# View thread priorities
chrt -p <tid>

# View thread names
cat /proc/<pid>/task/<tid>/comm
```

## Important Notes

1. **Root Privileges**: Setting real-time priorities (SCHED_FIFO/SCHED_RR) requires root
2. **Error Checking**: Always check return values from pthread functions
3. **Portability**: Some features (like gettid() and prctl()) are Linux-specific
4. **Resource Limits**: Check system limits with `ulimit -a`

## Compilation

Compile with:
```bash
g++ -std=c++11 -pthread thread_control.cpp -o thread_control
```

Run with sudo for real-time priorities:
```bash
sudo ./thread_control
```

Let me break down each aspect of advanced thread control with pthreads, one by one:

## 1. Thread Affinity (Core Binding)

**What it does**: Pins a thread to specific CPU core(s)

**Key Components**:
- `cpu_set_t`: Bitmask representing CPU cores
- `CPU_ZERO()`: Clears the bitmask
- `CPU_SET(n)`: Sets the bit for core `n`
- `pthread_setaffinity_np()`: Applies the affinity

**Example**:
```cpp
cpu_set_t cpuset;
CPU_ZERO(&cpuset);        // Start with no cores
CPU_SET(3, &cpuset);      // Allow only core 3
pthread_setaffinity_np(thread, sizeof(cpuset), &cpuset);
```

**Why use it**:
- Improves cache locality
- Prevents costly core migrations
- Essential for real-time systems

## 2. Thread Identification

**Three types of IDs**:
1. Process ID (PID): `getpid()` - Shared by all threads
2. Thread ID (TID): `gettid()` - Linux-specific unique ID
3. pthread ID: `pthread_self()` - Portable but opaque

**Example**:
```cpp
printf("PID=%d, TID=%d, pthread_t=%lu\n",
       getpid(), gettid(), pthread_self());
```

**Important**:
- `gettid()` is Linux-specific (use `syscall(SYS_gettid)` for portability)
- `pthread_t` can't be directly compared across processes

## 3. Thread Naming

**Linux-specific method**:
```cpp
prctl(PR_SET_NAME, "MyThread", 0, 0, 0);
```

**Why name threads**:
- Easier debugging (`ps -T`, `gdb`)
- Better log readability
- System monitoring clarity

**View thread names**:
```bash
cat /proc/$PID/task/$TID/comm
```

## 4. Thread Scheduling

**Policies**:
- `SCHED_OTHER`: Default time-sharing
- `SCHED_FIFO`: Real-time (no timeslices)
- `SCHED_RR`: Real-time with round-robin

**Priority Ranges**:
- 1 (lowest) to 99 (highest) for real-time policies

**Example**:
```cpp
struct sched_param param;
param.sched_priority = 80;
pthread_setschedparam(thread, SCHED_FIFO, &param);
```

**Requirements**:
- Root privileges needed for real-time policies
- Set `ulimit -r` for maximum priority

## 5. Complete Control Flow

1. **Create thread config**:
   ```cpp
   struct ThreadConfig {
       int core_id;
       int priority;
       const char* name;
   };
   ```

2. **Thread startup**:
   ```cpp
   void* thread_func(void* arg) {
       ThreadConfig* cfg = (ThreadConfig*)arg;
       set_name(cfg->name);
       set_affinity(cfg->core_id);
       set_priority(cfg->priority);
       // ... work ...
   }
   ```

3. **Create threads**:
   ```cpp
   pthread_t thread;
   ThreadConfig cfg = {2, 50, "Worker"};
   pthread_create(&thread, NULL, thread_func, &cfg);
   ```

## 6. System Monitoring Commands

**View thread/core assignment**:
```bash
ps -eLo pid,tid,psr,cmd | grep your_program
```
- `psr` shows current CPU core

**Check priorities**:
```bash
chrt -p $TID
```

**View all thread info**:
```bash
ls /proc/$PID/task/
```

## Important Considerations

1. **Error Handling**:
   Always check pthread function returns:
   ```cpp
   if (pthread_setaffinity_np(...) != 0) {
       perror("setaffinity failed");
   }
   ```

2. **Portability**:
   - Affinity (`pthread_setaffinity_np`) is non-portable
   - `gettid()` and `prctl()` are Linux-specific

3. **Security**:
   - Real-time scheduling requires root
   - Improper priority settings can hang systems

4. **Performance**:
   - Over-subscription hurts performance
   - Proper affinity improves cache utilization



Here are additional low-level controls available in pthreads for precise thread management:

---

### **1. Thread Stack Control**
#### Custom Stack Allocation
```cpp
void* custom_stack = malloc(2 * 1024 * 1024); // 2MB stack
pthread_attr_t attr;
pthread_attr_init(&attr);
pthread_attr_setstack(&attr, custom_stack, 2 * 1024 * 1024);

pthread_t thread;
pthread_create(&thread, &attr, thread_func, NULL);
pthread_attr_destroy(&attr);
```
- **Why**: Avoid default stack overflows for deep recursion
- **Key Functions**:
  - `pthread_attr_setstack()`: Fully custom stack
  - `pthread_attr_setstacksize()`: Only resize default stack

---

### **2. Thread Cancellation**
#### Controlled Termination
```cpp
void* thread_func(void*) {
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
    
    while(1) {
        pthread_testcancel(); // Cancellation point
        // Work...
    }
}

// From another thread:
pthread_cancel(thread);
```
- **Modes**:
  - `PTHREAD_CANCEL_DEFERRED`: Safe cleanup (default)
  - `PTHREAD_CANCEL_ASYNCHRONOUS`: Immediate (dangerous)
- **Cleanup Handlers**:
  ```cpp
  void cleanup(void* arg) { /* release resources */ }

  pthread_cleanup_push(cleanup, NULL);
  // Critical section
  pthread_cleanup_pop(1); // 1=execute, 0=don't execute
  ```

---

### **3. Thread-Specific Data (TSD)**
#### Per-Thread Storage
```cpp
pthread_key_t key;

void init_key() {
    pthread_key_create(&key, [](void* data) {
        free(data); // Destructor
    });
}

void* thread_func(void*) {
    int* data = malloc(sizeof(int));
    *data = 42;
    pthread_setspecific(key, data);
    
    // Later...
    int* stored = (int*)pthread_getspecific(key);
}
```
- **Use Case**: Thread-local variables without `thread_local` (C++11)

---

### **4. Signal Control**
#### Thread-Directed Signals
```cpp
void handler(int sig) { /* ... */ }

void* thread_func(void*) {
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    pthread_sigmask(SIG_BLOCK, &mask, NULL);
    
    struct sigaction sa;
    sa.sa_handler = handler;
    sigaction(SIGUSR2, &sa, NULL);
}

// Send signal to specific thread:
pthread_kill(thread, SIGUSR2);
```
- **Key Points**:
  - `pthread_sigmask()`: Block signals per-thread
  - `pthread_kill()`: Target specific thread

---

### **5. Thread Scheduling Controllers**
#### Explicit Yield
```cpp
pthread_yield(); // Or sched_yield()
```
#### Priority Inheritance
```cpp
pthread_mutexattr_t attr;
pthread_mutexattr_init(&attr);
pthread_mutexattr_setprotocol(&attr, PTHREAD_PRIO_INHERIT);
pthread_mutex_t mutex;
pthread_mutex_init(&mutex, &attr);
```
- **Prevents priority inversion** in real-time systems

---

### **6. CPU Time Monitoring**
#### Thread-Specific CPU Clock
```cpp
#include <time.h>
clockid_t cid;
pthread_getcpuclockid(thread, &cid);

struct timespec ts;
clock_gettime(cid, &ts);
printf("CPU time: %ld.%09ld sec\n", ts.tv_sec, ts.tv_nsec);
```

---

### **7. Barrier Synchronization**
#### Multi-Thread Rendezvous
```cpp
pthread_barrier_t barrier;
pthread_barrier_init(&barrier, NULL, 3); // 3 threads

void* thread_func(void*) {
    // Work...
    pthread_barrier_wait(&barrier);
    // Continue after all reach barrier
}
```

---

### **8. Spinlocks (For Very Short Critical Sections)**
```cpp
pthread_spinlock_t spinlock;
pthread_spin_init(&spinlock, PTHREAD_PROCESS_PRIVATE);

pthread_spin_lock(&spinlock);
// Ultra-fast critical section
pthread_spin_unlock(&spinlock);
```
- **Use When**: Lock duration < context switch time

---

### **9. Thread State Inspection (Linux-Specific)**
#### Query Thread Status
```cpp
#include <sys/syscall.h>
pid_t tid = syscall(SYS_gettid);

char stat[256];
snprintf(stat, sizeof(stat), "/proc/%d/task/%d/stat", getpid(), tid);
FILE* f = fopen(stat, "r");
// Parse thread state (R=running, S=sleeping, etc.)
```

---

### **10. NUMA Control (Advanced)**
#### Bind to NUMA Node
```cpp
#include <numa.h>
numa_run_on_node(1); // Run on NUMA node 1
numa_set_preferred(1); // Allocate memory here
```

---

### **When to Use Low-Level Controls**
| Control                  | Typical Use Case                          |
|--------------------------|------------------------------------------|
| Stack Control            | Deep recursion/embedded systems          |
| Cancellation             | Long-running task interruption           |
| TSD                      | Legacy code, C compatibility             |
| Signal Control           | Real-time event handling                 |
| Spinlocks                | Microsecond-critical sections            |
| NUMA Control             | High-performance computing               |

**Golden Rule**: Always prefer higher-level abstractions (like C++'s `<thread>`) unless you have specific low-level requirements. These controls are most valuable for:
- Real-time systems
- HPC applications
- Embedded systems
- OS development