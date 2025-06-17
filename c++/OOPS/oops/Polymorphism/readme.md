# Polymorphism in C++

Polymorphism is a core concept in object-oriented programming that allows objects of different types to be treated as objects of a common super type. In C++, there are two main types of polymorphism:

## 1. Compile-time Polymorphism (Static Polymorphism)

This is resolved during compile time and includes:

### a. Function Overloading
```cpp
#include <iostream>
using namespace std;

class Math {
public:
    int add(int a, int b) {
        return a + b;
    }
    
    double add(double a, double b) {
        return a + b;
    }
    
    int add(int a, int b, int c) {
        return a + b + c;
    }
};

int main() {
    Math m;
    cout << m.add(2, 3) << endl;       // Calls int add(int, int)
    cout << m.add(2.5, 3.7) << endl;   // Calls double add(double, double)
    cout << m.add(1, 2, 3) << endl;    // Calls int add(int, int, int)
    return 0;
}
```

### b. Operator Overloading
```cpp
#include <iostream>
using namespace std;

class Complex {
    double real, imag;
public:
    Complex(double r = 0, double i = 0) : real(r), imag(i) {}
    
    Complex operator + (const Complex& obj) {
        Complex res;
        res.real = real + obj.real;
        res.imag = imag + obj.imag;
        return res;
    }
    
    void display() {
        cout << real << " + " << imag << "i" << endl;
    }
};

int main() {
    Complex c1(1.5, 2.5), c2(3.1, 4.2);
    Complex c3 = c1 + c2;  // Operator overloading
    c3.display();
    return 0;
}
```

### c. Template Polymorphism
```cpp
#include <iostream>
using namespace std;

template <typename T>
T getMax(T a, T b) {
    return (a > b) ? a : b;
}

int main() {
    cout << getMax<int>(3, 7) << endl;       // 7
    cout << getMax<double>(3.5, 2.9) << endl; // 3.5
    cout << getMax<char>('a', 'z') << endl;   // z
    return 0;
}
```

## 2. Runtime Polymorphism (Dynamic Polymorphism)

This is resolved during runtime and is achieved through:

### a. Virtual Functions and Inheritance
```cpp
#include <iostream>
using namespace std;

class Animal {
public:
    virtual void makeSound() {
        cout << "Some animal sound" << endl;
    }
};

class Dog : public Animal {
public:
    void makeSound() override {
        cout << "Woof woof!" << endl;
    }
};

class Cat : public Animal {
public:
    void makeSound() override {
        cout << "Meow!" << endl;
    }
};

int main() {
    Animal* animals[3];
    animals[0] = new Animal();
    animals[1] = new Dog();
    animals[2] = new Cat();
    
    for (int i = 0; i < 3; i++) {
        animals[i]->makeSound();  // Runtime polymorphism
    }
    
    // Cleanup
    for (int i = 0; i < 3; i++) {
        delete animals[i];
    }
    
    return 0;
}
```

### b. Pure Virtual Functions and Abstract Classes
```cpp
#include <iostream>
using namespace std;

class Shape {
public:
    virtual float area() = 0;  // Pure virtual function
    virtual float perimeter() = 0;
};

class Circle : public Shape {
    float radius;
public:
    Circle(float r) : radius(r) {}
    
    float area() override {
        return 3.14159 * radius * radius;
    }
    
    float perimeter() override {
        return 2 * 3.14159 * radius;
    }
};

class Rectangle : public Shape {
    float length, width;
public:
    Rectangle(float l, float w) : length(l), width(w) {}
    
    float area() override {
        return length * width;
    }
    
    float perimeter() override {
        return 2 * (length + width);
    }
};

int main() {
    Shape* shapes[2];
    shapes[0] = new Circle(5);
    shapes[1] = new Rectangle(4, 6);
    
    for (int i = 0; i < 2; i++) {
        cout << "Area: " << shapes[i]->area() << endl;
        cout << "Perimeter: " << shapes[i]->perimeter() << endl;
    }
    
    // Cleanup
    for (int i = 0; i < 2; i++) {
        delete shapes[i];
    }
    
    return 0;
}
```

## Key Differences

| Compile-time Polymorphism | Runtime Polymorphism |
|---------------------------|----------------------|
| Resolved at compile time | Resolved at runtime |
| Faster execution | Slower due to vtable lookup |
| Examples: overloading, templates | Examples: virtual functions |
| No inheritance required | Requires inheritance |
| Flexibility in code structure | Flexibility in object behavior |

Polymorphism allows for more flexible and maintainable code by enabling a single interface to represent different underlying forms (data types).