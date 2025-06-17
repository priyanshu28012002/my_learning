#include <iostream>
#include <thread>
#include <future>

void worker(std::promise<std::string>&& prom) {
    std::cout << "[Worker] Doing some work...\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));  // Simulate work
    
    // Signal completion by setting a value in the promise
    prom.set_value("42");  // Send result back to main thread
    std::cout << "[Worker] Signal sent (value = 42)\n";
}

int main() {
    // Create a promise to receive a future signal
    std::promise<std::string> prom;
    std::future<std::string> fut = prom.get_future();  // Get the associated future

    // Start worker thread (move the promise)
    std::thread th(worker, std::move(prom));
    // We use std::move because std::promise cannot be copied—it’s a unique resource that represents ownership of a thread synchronization state. 
    // Moving it transfers ownership safely to the worker thread, ensuring only one thread controls the promise.

    std::cout << "[Main] Waiting for signal...\n";
    
    // Block until the worker sets the value
    std::string result = fut.get();  // Waits here until the promise is fulfilled
    // th.join();   work with both detach and join
    th.detach();
    std::cout << "[Main] Received signal! Result = " << result << "\n";

   
    return 0;
}