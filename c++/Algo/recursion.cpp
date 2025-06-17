// factorial_recursion.cpp
// Demonstrates both classical and tail recursion

#include <iostream>

// Classical recursion: NOT tail-recursive
int classicalRec(int x) {
    if (x <= 1)
        return 1;
    return x * classicalRec(x - 1); // The multiplication happens after the recursive call
}

// Tail recursion: optimized version
int tailRec(int x, int product) {
    if (x <= 1)
        return product;
    return tailRec(x - 1, product * x); // Tail call: recursive call is the last action
}

int main() {
    int x = 4;

    std::cout << "Factorial using classical recursion: " << classicalRec(x) << std::endl;
    std::cout << "Factorial using tail recursion: " << tailRec(x, 1) << std::endl;

    return 0;
}
