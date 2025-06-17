#include <iostream>
#include <future>
#include <chrono>

int slow_computation() {
    std::this_thread::sleep_for(std::chrono::seconds(5)); // Simulate long work
    return 42;
}

int main() {
    std::future<int> fut = std::async(std::launch::async, slow_computation);

    std::cout << "Waiting for result (max 2 seconds)...\n";

    // Wait for 2 seconds
    auto status = fut.wait_for(std::chrono::seconds(2));

    if (status == std::future_status::ready) {
        std::cout << "Result ready: " << fut.get() << "\n";
    } 
    else if (status == std::future_status::timeout) {
        std::cout << "Timeout! Task still running...\n";
    } 
    else if (status == std::future_status::deferred) {
        std::cout << "Task not yet started (deferred).\n";
    }

    return 0;
}