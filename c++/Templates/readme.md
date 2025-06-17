# Templates
In C++, a **template** is a powerful feature that allows you to write **generic** and **reusable** code. It enables you to define functions or classes that can operate with **any data type** without rewriting the code for each specific type.

---

### 🔹 Types of Templates

In C++, there are several **types of templates**, each serving a specific use case in **generic programming**. Here's a breakdown of the **main types of templates**:

---

### 🔹 1. **Function Template**

Used to create a generic function that works with any data type.

```cpp
template <typename T>
T max(T a, T b) {
    return (a > b) ? a : b;
}
```

✅ Use when you want the **same logic** for multiple types.

---

### 🔹 2. **Class Template**

Used to define a generic class or data structure.

```cpp
template <class T>
class Box {
    T value;
public:
    void set(T v) { value = v; }
    T get() { return value; }
};
```

✅ Use when you want a **data structure** that works with any type (like `std::vector<T>`).

---

### 🔹 3. **Template with Multiple Parameters**

You can have more than one type or non-type parameter.

```cpp
template <typename T, typename U>
class Pair {
public:
    T first;
    U second;
};
```

✅ Allows combinations of different types.

---

### 🔹 4. **Non-Type Template Parameters**

Templates that take constant values (not types).

```cpp
template <typename T, int size>
class Array {
    T arr[size];
};
```

✅ Useful for **fixed-size arrays** or configurations known at compile-time.

---

### 🔹 5. **Variadic Templates** (C++11 and later)

Allows templates to accept **any number of template arguments**.

```cpp
template<typename... Args>
void printAll(Args... args) {
    (std::cout << ... << args); // Fold expression
}
```

✅ Very powerful for functions/classes with **variable parameters** (like in `std::tuple` or `std::make_shared`).

---

### 🔹 6. **Template Specialization**

You can provide a **custom implementation** of a template for a specific type.

#### ✔ Full Specialization

```cpp
template<>
class Box<int> {
    // Specialized implementation for int
};
```

#### ✔ Partial Specialization

```cpp
template<typename T>
class Box<T*> {
    // Specialized for pointer types
};
```

✅ Use when a specific type needs **different behavior**.

---

### Summary Table

| Template Type               | Description                           | Example Syntax                    |
| --------------------------- | ------------------------------------- | --------------------------------- |
| Function Template           | Generic functions                     | `template<typename T> T func()`   |
| Class Template              | Generic classes                       | `template<class T> class MyClass` |
| Multiple Parameters         | Multiple type or non-type parameters  | `template<typename T, int N>`     |
| Non-Type Template Parameter | Pass values as parameters             | `template<int N>`                 |
| Variadic Template           | Accepts unlimited template parameters | `template<typename... Args>`      |
| Template Specialization     | Custom behavior for specific types    | `template<> class MyClass<int>`   |

---
