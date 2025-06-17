# **Encapsulation in C++ (OOPs Concept)**

Encapsulation is one of the **four fundamental principles of Object-Oriented Programming (OOP)** in C++. It refers to **binding data (attributes) and methods (functions) that operate on the data into a single unit (class)** while restricting direct access to some of the object's components.

## **Key Features of Encapsulation**
1. **Data Hiding**: Restricting direct access to class members.
2. **Controlled Access**: Providing **getter and setter** methods to modify and view data.
3. **Security**: Prevents unauthorized modifications.
4. **Modularity**: Keeps related data and functions together.

---

## **How to Achieve Encapsulation in C++?**
Encapsulation is implemented using:
- **`private`** → (Data is hidden, accessible only within the class)
- **`public`** → (Methods to access/modify `private` data)
- **`protected`** → (Accessible within the class and derived classes)

### **Example 1: Basic Encapsulation**
```cpp
#include <iostream>
using namespace std;

class Employee {
private:  
    // Private data (hidden from outside)
    string name;
    int salary;

public:
    // Setter methods (to modify private data)
    void setName(string n) {
        name = n;
    }
    void setSalary(int s) {
        salary = s;
    }

    // Getter methods (to access private data)
    string getName() {
        return name;
    }
    int getSalary() {
        return salary;
    }
};

int main() {
    Employee emp;
    emp.setName("John Doe");
    emp.setSalary(50000);

    cout << "Name: " << emp.getName() << endl;
    cout << "Salary: " << emp.getSalary() << endl;

    // emp.salary = 60000; // Error: 'salary' is private
    return 0;
}
```
**Output:**
```
Name: John Doe
Salary: 50000
```

### **Example 2: Encapsulation with Validation**
```cpp
#include <iostream>
using namespace std;

class BankAccount {
private:
    double balance;

public:
    void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            cout << "Deposit successful. New balance: " << balance << endl;
        } else {
            cout << "Invalid amount!" << endl;
        }
    }

    void withdraw(double amount) {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            cout << "Withdrawal successful. Remaining balance: " << balance << endl;
        } else {
            cout << "Insufficient funds or invalid amount!" << endl;
        }
    }

    double getBalance() {
        return balance;
    }
};

int main() {
    BankAccount acc;
    acc.deposit(1000);
    acc.withdraw(500);
    acc.withdraw(600); // Fails (insufficient funds)
    cout << "Final balance: " << acc.getBalance() << endl;
    return 0;
}
```
**Output:**
```
Deposit successful. New balance: 1000
Withdrawal successful. Remaining balance: 500
Insufficient funds or invalid amount!
Final balance: 500
```

---

## **Advantages of Encapsulation**
✔ **Security**: Prevents unauthorized access to data.  
✔ **Control**: Allows validation before modifying data.  
✔ **Flexibility**: Internal changes don’t affect external code.  
✔ **Reusability**: Encapsulated classes can be reused easily.  

---

## **Difference Between Encapsulation & Abstraction**
| **Encapsulation** | **Abstraction** |
|-------------------|----------------|
| Hides data by making variables `private` | Hides implementation details |
| Uses **getters/setters** for controlled access | Uses **abstract classes/interfaces** |
| Protects data integrity | Simplifies complex systems |
| Example: `private` variables in a `class` | Example: Pure virtual functions (`virtual void func() = 0`) |

---

## **Conclusion**
- Encapsulation **bundles data and methods** inside a class.
- It **protects data** from unauthorized access.
- **Getters and setters** provide controlled access to `private` members.
- Improves **security, maintainability, and reusability** in C++ programs.

Would you like a real-world analogy or more advanced examples? 😊