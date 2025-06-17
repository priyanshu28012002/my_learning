// #include <iostream>
// #include <thread>
// #include <vector>
// #include <pthread.h>
// #include <sched.h>
// #include <unistd.h> // for sysconf()

// // Function to set CPU affinity for a thread
// void set_core_affinity(pthread_t thread, int thread_id) {
//     cpu_set_t cpuset;
//     CPU_ZERO(&cpuset);
//     CPU_SET(thread_id, &cpuset);
//     pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset);
// }

// // Function to get the number of available thread
// int get_available_thread() {
//     // Method 1: C++ standard way (may return logical thread)
//     int hw_thread = std::thread::hardware_concurrency();
//     return hw_thread;
// }

// // Thread work function
// void pinned_thread_work(int thread_id) {
//     // Set affinity for this thread
//     set_core_affinity(pthread_self(), thread_id);
    
//     // Intensive work to verify core pinning
//     volatile double result = 0;
//     for (int i = 0; i < 100000000; i++) {
//         result += i * 0.1;
//     }
    
//     std::cout << "Thread " << thread_id << " completed work. Result: " << result << "\n";
// }

// int main() {
//     // Get available physical thread
//     int num_thread = get_available_thread();
//     std::cout << "Available thread: " << num_thread << std::endl;
    
//     // Create one thread per core
//     std::vector<std::thread> threads;
//     for (int i = 0; i < num_thread; i++) {
//         threads.emplace_back(pinned_thread_work, i);
//     }
    
//     // Wait for all threads to complete
//     for (auto& t : threads) {
//         t.join();
//     }
    
//     std::cout << "All threads completed.\n";
//     return 0;
// }
#include <iostream>
#include <thread>
#include <vector>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>
#include <cmath>
#include <atomic>

// Atomic counter for thread completion
std::atomic<int> completed_threads(0);

// Function to set CPU affinity
void set_core_affinity(pthread_t thread, int thread_id) {
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(thread_id, &cpuset);
    pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset);
}

// Function to get available threads
int get_available_threads() {
    return std::thread::hardware_concurrency();
}

// Heavy computational workload
void heavy_computation(int thread_id) {
    set_core_affinity(pthread_self(), thread_id);
    
    const int iterations = 500000000; // Increased workload 5x
    double result = 0;
    
    // Multiple heavy math operations
    for (int i = 0; i < iterations; i++) {
        result += std::sqrt(std::log(i+1) * std::sin(i) * std::cos(i));
        result -= std::pow(std::fmod(i, 100), 0.5);
        if (i % 10000000 == 0) {
            std::cout << "Thread " << thread_id << " progress: " 
                      << (i * 100.0 / iterations) << "%\n";
        }
    }
    
    completed_threads++;
    std::cout << "Thread " << thread_id << " FINAL result: " << result << "\n";
}

// Memory-intensive workload
void memory_intensive_work(int thread_id) {
    set_core_affinity(pthread_self(), thread_id);
    
    const size_t buffer_size = 10000000; // 10 million elements
    std::vector<double> buffer(buffer_size);
    
    // Fill and process memory
    for (size_t i = 0; i < buffer_size; i++) {
        buffer[i] = std::sin(i) * std::cos(i);
        if (i % 1000000 == 0) {
            std::sort(buffer.begin(), buffer.begin() + i + 1000);
        }
    }
    
    completed_threads++;
    std::cout << "Thread " << thread_id << " processed " 
              << buffer_size << " elements\n";
}

int main() {
    const int num_threads = get_available_threads();
    std::cout << "Available threads: " << num_threads << "\n";
    std::cout << "Starting heavy workloads...\n";
    
    std::vector<std::thread> threads;
    
    // Alternate between computation and memory workloads
    for (int i = 0; i < num_threads; i++) {
       
            threads.emplace_back(heavy_computation, i);
        // } else {
        //     threads.emplace_back(memory_intensive_work, i);
        // }
    }
    
    // Progress monitor
    while (completed_threads < num_threads) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "Progress: " << completed_threads << "/" 
                  << num_threads << " threads completed\n";
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    std::cout << "All threads completed. Total CPU stress achieved.\n";
    return 0;
}