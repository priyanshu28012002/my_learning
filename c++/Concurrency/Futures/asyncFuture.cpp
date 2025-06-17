#include <iostream>
#include <future>
#include <thread>

int compute() {
    std::this_thread::sleep_for(std::chrono::seconds(2)); // Simulate work
    return 42;
}

int main() {
    // Launch async task and get a future
    std::future<int> fut = std::async(std::launch::async, compute);

    std::cout << "Waiting for result...\n";
    
    // Blocks until the result is ready
     std::cout << "valid furture: " << fut.valid() << "\n";
    int result = fut.get(); 
     std::cout << "valid furture: " << fut.valid() << "\n";

    std::cout << "Result: " << result << "\n";
    return 0;
}