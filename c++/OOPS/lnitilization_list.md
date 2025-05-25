# Initialization Lists in C++: Working and Benefits

## How Initialization Lists Work

In your example, you've shown two versions of class `B` that initialize member `a` differently:

1. **Using initialization list**:
```cpp
class B {
private:
    A a;
public:
    B() : a(4) {}  // Initialization list
};
```

2. **Using assignment in constructor body**:
```cpp
class B {
private:
    A a;
public:
    B() {
        a = 4;  // Assignment in constructor body
    }
};
```

## Key Differences and Benefits

1. **Order of Operations**:
   - With initialization list: `A`'s constructor `A(int)` is called directly
   - With assignment: `A`'s default constructor is called first, then assignment happens

2. **Performance Benefit**:
   - Initialization list avoids the extra step of default construction followed by assignment
   - For your example:
     - Initialization list version: Just calls `A(int)` once
     - Assignment version: Calls `A()` then uses assignment operator

3. **Required Cases**:
   - Must use initialization lists for:
     - const members
     - reference members
     - members without default constructors
     - base classes without default constructors

4. **Initialization Order**:
   - Members are initialized in the order they're declared in the class, not the order in the initialization list

## Best Practice

Always prefer initialization lists because:
- They're more efficient
- They work in all cases (including those where assignment won't work)
- They make the initialization explicit and clear

For your specific example, the initialization list version (`B() : a(4) {}`) is more efficient as it constructs `a` directly with the value 4, while the other version first default-constructs `a` and then assigns it the value 4.