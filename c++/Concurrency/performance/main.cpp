#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <mutex>
#include <cmath>

// std::mutex mtx;

// void workload(int iterations) {
//     // Simulate CPU-bound work
//     for (int i = 0; i < iterations; i++) {
//         d
//     std::lock_guard<std::mutex> lock(mtx);
//     std::cout << thread_count << " threads: " << duration << " ms\n";
// }

// int main() {
//     const int max_threads = 2 * std::thread::hardware_concurrency();
//     const int work_per_thread = 10000000;
    
//     std::cout << "Thread scaling benchmark:\n";
//     for (int t = 1; t <= max_threads; t++) {
//         benchmark(t, work_per_thread);
//     }
    
//     return 0;
// }ouble x = std::sin(i) * std::cos(i);
//     }
// }

// void benchmark(int thread_count, int work_per_thread) {
//     auto 
//     std::lock_guard<std::mutex> lock(mtx);
//     std::cout << thread_count << " threads: " << duration << " ms\n";
// }

// int main() {
//     const int max_threads = 2 * std::thread::hardware_concurrency();
//     const int work_per_thread = 10000000;
    
//     std::cout << "Thread scaling benchmark:\n";
//     for (int t = 1; t <= max_threads; t++) {
//         benchmark(t, work_per_thread);
//     }
    
//     return 0;
// }start = std::chrono::high_resolution_clock::now();
    
//     std::vector<std::thread> threads;
//     for (int i = 0; i < thread_count; i++) {
//         threads.emplace_back(workload, work_per_thread);
//     }
    
//     for (auto& t : threads) {
//         t.join();
//     }
    
//     auto end = std::chrono::high_resolution_clock::now();
//     auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    
//     std::lock_guard<std::mutex> lock(mtx);
//     std::cout << thread_count << " threads: " << duration << " ms\n";
// }

// int main() {
//     const int max_threads = 2 * std::thread::hardware_concurrency();
//     const int work_per_thread = 10000000;
    
//     std::cout << "Thread scaling benchmark:\n";
//     for (int t = 1; t <= max_threads; t++) {
//         benchmark(t, work_per_thread);
//     }
    
//     return 0;
// }
int calculate_optimal_threads(bool cpu_bound = true) {
    const unsigned int cores = std::thread::hardware_concurrency();
    
    if (cpu_bound) {
        return cores + 1;
    } else {
        // For I/O bound, estimate wait vs compute ratio
        double wait_ratio = 0.8; // Example: 80% waiting
        return static_cast<int>(cores / (1 - wait_ratio));
    }
}

// Usage:
int main() {
    bool is_cpu_bound = true; // Set based on your application
    int optimal_threads = calculate_optimal_threads(is_cpu_bound);
    
    std::cout << "Optimal threads: " << optimal_threads << std::endl;
    
    // Create thread pool with optimal size
    std::vector<std::thread> thread_pool;
    thread_pool.reserve(optimal_threads);
    std::this_thread::sleep_for(std::chrono::milliseconds(21));
    // ... rest of your application logic
    return 0;
}