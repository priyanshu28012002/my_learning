#include <iostream>
#include <future>
#include <thread>

int compute()
{
    std::cout << "Running in thread: " << std::this_thread::get_id() << "\n";
    return 42;
}

int main()
{
    // std::launch::async Launch task immediately in a new thread
    // std::launch::deferred Task is deferred (runs only when get() is called)
    // auto fut = std::async(compute);  // Default: usually std::launch::async
    // std::async(std::launch::async | std::launch::deferred, task);


    std::future<int> fut = std::async(std::launch::deferred, compute);

    std::cout << "Main thread: " << std::this_thread::get_id() << "\n";
    std::cout << "Result: " << fut.get() << "\n"; // Blocks if not ready

    return 0;
}