#include <iostream>
#include <future>
#include <chrono>

int slow_computation() {
    std::this_thread::sleep_for(std::chrono::seconds(3)); // Simulate work
    return 100;
}

int main() {
    std::future<int> fut = std::async(std::launch::async, slow_computation);

    auto deadline = std::chrono::system_clock::now() + std::chrono::seconds(2);
    std::cout << "Waiting until deadline (2 seconds from now)...\n";

    auto status = fut.wait_until(deadline);

    if (status == std::future_status::ready) {
        std::cout << "Result ready: " << fut.get() << "\n";
    } 
    else if (status == std::future_status::timeout) {
        std::cout << "Deadline reached! Task still running...\n";
    }

    return 0;
}