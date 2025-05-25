#include <iostream>
#include <thread>
#include <pthread.h>
#include <sched.h>
#include <cstring>
#include <unistd.h>

void threadFunction() {
    std::cout << "Thread running\n";
    while (true) {
        // Simulate work
        usleep(100000);
    }
}

int main() {
    // Create a thread
    std::thread t(threadFunction);

    // Ensure the thread is joinable before accessing its native handle
    if (t.joinable()) {
        // Set thread name
        pthread_setname_np(t.native_handle(), "MyThread");

        // Set thread priority (real-time priority)
        sched_param sch;
        sch.sched_priority = 10; // Set to a value between 1 and 99
        if (pthread_setschedparam(t.native_handle(), SCHED_FIFO, &sch)) {
            std::cerr << "Failed to set thread priority: " << strerror(errno) << std::endl;
        } else {
            std::cout << "Thread priority set to real-time\n";
        }

        // Set CPU affinity (bind thread to CPU core 0)
        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        CPU_SET(0, &cpuset);
        if (pthread_setaffinity_np(t.native_handle(), sizeof(cpu_set_t), &cpuset)) {
            std::cerr << "Failed to set CPU affinity: " << strerror(errno) << std::endl;
        } else {
            std::cout << "Thread bound to CPU core 0\n";
        }

        // Join the thread
        t.join();
    } else {
        std::cerr << "Thread is not joinable\n";
    }

    return 0;
}
