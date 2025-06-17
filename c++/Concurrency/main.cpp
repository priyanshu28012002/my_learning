#include <iostream>
#include <jthread>
#include <chrono>
#include <utility>
#include <string>

// Functor struct: a callable object
template <typename T>
struct ThreadFunction {
    T m_data;

    ThreadFunction(T data) : m_data(data) {}

    void operator()() const {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "Thread function running with value: " << m_data << "\n";
    }
};

int main() {
    // --- Constructor: create a thread using a struct functor
    std::thread t1(ThreadFunction<std::string>("string"));
    std::thread t9;     
t9 = std::move(t1);


    // --- Observers: joinable
    if (t1.joinable()) {
        std::cout << "t1 is joinable.\n";
         // --- get_id: retrieve thread ID
        std::thread::id t1_id = t1.get_id();
        std::cout << "t1 ID: " << t1_id << "\n";

        // --- native_handle (implementation-specific)
        auto handle = t1.native_handle(); // No output, just demo

        // --- hardware_concurrency
        unsigned int cores = std::thread::hardware_concurrency();
        std::cout << "Hardware concurrency (cores): " << cores << "\n";

        // --- join: wait for the thread to finish
        t1.join();
        std::cout << "t1 joined.\n";
    }

     if (t9.joinable()) {
        t9.join();             // Join the moved thread
    }

   

    // --- Swap example
    std::thread t2(ThreadFunction<int>(100));
    std::thread t3(ThreadFunction<int>(200));

    std::cout << "Swapping t2 and t3...\n";
    t2.swap(t3); // Swap member function

    if (t2.joinable()) t2.join();
    if (t3.joinable()) t3.join();

    // --- Move assignment operator
    std::thread t4(ThreadFunction<int>(300));
    std::thread t5 = std::move(t4); // operator=
    if (t5.joinable()) t5.join();

    // --- detach: thread runs independently
    std::thread t6(ThreadFunction<std::string>("detached"));
    t6.detach();

    // --- std::swap (non-member function)
    std::thread t7(ThreadFunction<int>(400));
    std::thread t8(ThreadFunction<int>(500));
    std::swap(t7, t8);

    if (t7.joinable()) t7.join();
    if (t8.joinable()) t8.join();

    // Wait to allow detached thread to finish
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "Main finished.\n";

    return 0;
}
