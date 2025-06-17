#include <iostream>
#include <future>
#include <vector>

int compute(int x) {
    // Simulate computation
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    return x * x;
}

int main() {
    std::vector<std::future<int>> futures;
    
    // Launch async tasks
    for(int i = 0; i < 5; ++i) {
        futures.push_back(std::async(std::launch::async, compute, i));
    }
    
    // Get results
    for(int i = 0; i < 5; ++i) {
        std::cout << "Result " << i << ": " << futures[i].get() << std::endl;
    }
    
    return 0;
}