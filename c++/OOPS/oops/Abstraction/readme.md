# Abstraction in C++

Abstraction is a fundamental concept in object-oriented programming that focuses on hiding complex implementation details while exposing only the essential features to the user. In C++, abstraction is primarily achieved through abstract classes and interfaces.

## Key Concepts of Abstraction

1. **Hides implementation details**
2. **Shows only essential features**
3. **Reduces complexity**
4. **Increases reusability**

## Ways to Achieve Abstraction in C++

### 1. Using Classes (Basic Abstraction)

```cpp
#include <iostream>
using namespace std;

class BankAccount {
private:
    double balance;  // Hidden implementation detail

public:
    BankAccount(double initialBalance) : balance(initialBalance) {}

    void deposit(double amount) {
        balance += amount;
    }

    void withdraw(double amount) {
        if (amount <= balance) {
            balance -= amount;
        } else {
            cout << "Insufficient funds!" << endl;
        }
    }

    double getBalance() {
        return balance;
    }
};

int main() {
    BankAccount account(1000);
    account.deposit(500);
    account.withdraw(200);
    cout << "Current balance: " << account.getBalance() << endl;
    // account.balance = 10000; // Error: balance is private
    return 0;
}
```

### 2. Using Abstract Classes (Pure Virtual Functions)

```cpp
#include <iostream>
using namespace std;

// Abstract class
class Shape {
public:
    // Pure virtual functions (must be implemented by derived classes)
    virtual float calculateArea() = 0;
    virtual float calculatePerimeter() = 0;
    
    // Concrete method
    void display() {
        cout << "This is a shape" << endl;
    }
};

class Circle : public Shape {
    float radius;
public:
    Circle(float r) : radius(r) {}
    
    float calculateArea() override {
        return 3.14159 * radius * radius;
    }
    
    float calculatePerimeter() override {
        return 2 * 3.14159 * radius;
    }
};

class Rectangle : public Shape {
    float length, width;
public:
    Rectangle(float l, float w) : length(l), width(w) {}
    
    float calculateArea() override {
        return length * width;
    }
    
    float calculatePerimeter() override {
        return 2 * (length + width);
    }
};

int main() {
    // Shape s; // Error: cannot instantiate abstract class
    Shape* shapes[2];
    shapes[0] = new Circle(5);
    shapes[1] = new Rectangle(4, 6);
    
    for (int i = 0; i < 2; i++) {
        shapes[i]->display();
        cout << "Area: " << shapes[i]->calculateArea() << endl;
        cout << "Perimeter: " << shapes[i]->calculatePerimeter() << endl;
    }
    
    // Cleanup
    for (int i = 0; i < 2; i++) {
        delete shapes[i];
    }
    
    return 0;
}
```

### 3. Using Interfaces (All Pure Virtual Functions)

In C++, interfaces are implemented as classes with all pure virtual functions.

```cpp
#include <iostream>
using namespace std;

// Interface
class Drawable {
public:
    virtual void draw() = 0;
    virtual ~Drawable() {} // Virtual destructor for proper cleanup
};

class Circle : public Drawable {
public:
    void draw() override {
        cout << "Drawing a circle" << endl;
    }
};

class Square : public Drawable {
public:
    void draw() override {
        cout << "Drawing a square" << endl;
    }
};

void render(Drawable* d) {
    d->draw();
}

int main() {
    Drawable* shapes[2];
    shapes[0] = new Circle();
    shapes[1] = new Square();
    
    for (int i = 0; i < 2; i++) {
        render(shapes[i]);
    }
    
    // Cleanup
    for (int i = 0; i < 2; i++) {
        delete shapes[i];
    }
    
    return 0;
}
```

## Benefits of Abstraction

1. **Security**: Hides internal details and prevents direct access
2. **Maintainability**: Changes inside the class don't affect external code
3. **Simplicity**: Users interact with simple interfaces
4. **Reusability**: Abstract concepts can be implemented in multiple ways

## Real-world Examples

1. **Car Engine**: You use the accelerator without knowing combustion details
2. **Smartphone**: You make calls without understanding radio signal processing
3. **ATM Machine**: You withdraw money without knowing the banking system

## Abstract Class vs Interface

| Feature          | Abstract Class                | Interface                     |
|------------------|-------------------------------|-------------------------------|
| Methods          | Can have both concrete and pure virtual | Only pure virtual |
| Variables        | Can have member variables     | Only constants (in C++)       |
| Inheritance      | Single inheritance            | Multiple inheritance possible |
| Constructor      | Can have constructors         | No constructors               |
| Default Behavior | Can provide some implementation | No implementation provided |

Abstraction is a powerful tool in C++ that helps manage complexity in large software systems by focusing on what an object does rather than how it does it.