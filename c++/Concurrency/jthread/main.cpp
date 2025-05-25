#include <iostream>
#include <thread>
#include <chrono>

void worker(std::stop_token stoken) {
    int counter = 0;

    while (!stoken.stop_requested()) {
        std::cout << "Working... " << counter++ << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    std::cout << "Stop requested. Cleaning up...\n";
}

int main() {
    // Start a jthread with stop token support
    std::jthread jt(worker);  // Automatically passes stop_token to worker

    // ✅ Get the stop_source from the jthread
    std::stop_source ssource = jt.get_stop_source();  // Allows us to request stop

    // Let the thread work for a bit
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // ✅ Use the stop_source to request cancellation
    std::cout << "Main thread requesting stop...\n";
    ssource.request_stop();  // Signals the worker thread to stop

    // ✅ jthread will join automatically on destruction
    return 0;
}
