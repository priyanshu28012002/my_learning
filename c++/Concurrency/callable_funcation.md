# Callable Function
**one example for each callable type** that you can pass to a `std::thread`. All examples create a thread, pass the callable, and wait for it to finish using `join()`.

---

### ✅ 1. **Function**

```cpp
#include <iostream>
#include <thread>

void myFunction() {
    std::cout << "Thread running: Function\n";
}

int main() {
    std::thread t(myFunction);
    t.join();
    return 0;
}
```

---

### ✅ 2. **Lambda Function**

```cpp
#include <iostream>
#include <thread>

int main() {
    std::thread t([]() {
        std::cout << "Thread running: Lambda\n";
    });
    t.join();
    return 0;
}
```

---

### ✅ 3. **Functor (Struct with `operator()`)**

```cpp
#include <iostream>
#include <thread>

struct MyFunctor {
    void operator()() const {
        std::cout << "Thread running: Functor (struct)\n";
    }
};

int main() {
    std::thread t(MyFunctor());
    t.join();
    return 0;
}
```

---

### ✅ 4. **`std::bind` Result**

```cpp
#include <iostream>
#include <thread>
#include <functional>

void boundFunction(int x) {
    std::cout << "Thread running: std::bind, value = " << x << "\n";
}

int main() {
    auto bound = std::bind(boundFunction, 10);
    std::thread t(bound);
    t.join();
    return 0;
}
```

---

### ✅ 5. **Pointer to Member Function (with Object)**

```cpp
#include <iostream>
#include <thread>

class MyClass {
public:
    void memberFunction() {
        std::cout << "Thread running: Member Function\n";
    }
};

int main() {
    MyClass obj;
    std::thread t(&MyClass::memberFunction, &obj);  // pointer to member function + object pointer
    t.join();
    return 0;
}
```

---