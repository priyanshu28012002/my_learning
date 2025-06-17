#include <iostream>
#include <concepts>

// template <typename T>
// concept Addable = requires(T a, T b) {
//     a + b;
// };

// struct NotAddable {};

// template <Addable T>
// T add(T a, T b) {
//     return a + b;
// }


// template <typename T>
// requires Addable<T>
// T add(T a, T b) {
//     return a + b;
// }


template <typename T>
T add(T a, T b)
requires requires(T x, T y) {
    x + y;  // Must support addition
}
{
    return a + b;
}


int main() {
    int x = 5, y = 10;
    std::cout << "Sum: " << add(x, y) << std::endl;

    double a = 2.5, b = 3.5;
    std::cout << "Sum: " << add(a, b) << std::endl;

    // std::cout<< add("w1" , "e2");

  
    // NotAddable na1, na2;
    // add(na1, na2); // Compile-time error

    return 0;
}
