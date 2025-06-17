// counting 

// #include <iostream>
// #include <thread>
// #include <semaphore>
// #include <queue>
// #include <chrono>

// std::queue<int> buffer;
// constexpr int max_buffer_size = 5;

// std::counting_semaphore<max_buffer_size> empty_slots(max_buffer_size);  // Initially all empty
// std::counting_semaphore<max_buffer_size> full_slots(0);                // Initially no items
// std::mutex mtx;  // Protects buffer access

// void producer() {
//     for (int i = 1; i <= 10; ++i) {
//         empty_slots.acquire();  // Wait for an empty slot
        
//         {
//             std::lock_guard<std::mutex> lock(mtx);
//             buffer.push(i);
//             std::cout << "Produced: " << i << " | Buffer size: " << buffer.size() << "\n";
//         }
        
//         full_slots.release();  // Signal that a new item is available
//         std::this_thread::sleep_for(std::chrono::milliseconds(100));  // Simulate work
//     }
// }

// void consumer() {
//     for (int i = 1; i <= 10; ++i) {
//         full_slots.acquire();  // Wait for an available item
        
//         {
//             std::lock_guard<std::mutex> lock(mtx);
//             int item = buffer.front();
//             buffer.pop();
//             std::cout << "Consumed: " << item << " | Buffer size: " << buffer.size() << "\n";
//         }
        
//         empty_slots.release();  // Signal that a slot is now free
//         std::this_thread::sleep_for(std::chrono::milliseconds(200));  // Simulate work
//     }
// }

// int main() {
//     std::thread producer_thread(producer);
//     std::thread consumer_thread(consumer);

//     producer_thread.join();
//     consumer_thread.join();

//     return 0;
// }



/// Binary 
#include <iostream>
#include <thread>
#include <semaphore>
#include <vector>

constexpr int num_threads = 3;
std::binary_semaphore barrier_sem(0);  // Initially blocked
int arrived = 0;
std::mutex mtx;

void worker(int id) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100 * id));  // Simulate work
    
    {
        std::lock_guard<std::mutex> lock(mtx);
        arrived++;
        std::cout << "Thread " << id << " reached barrier (" << arrived << "/" << num_threads << ")\n";
        
        if (arrived == num_threads) {
            barrier_sem.release();  // Release all threads
        }
    }

    barrier_sem.acquire();          // Wait for the last thread
    barrier_sem.release();          // Allow others to proceed (releasing the semaphore back)
    
    std::cout << "Thread " << id << " passed barrier\n";
}

int main() {
    std::vector<std::thread> threads;
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(worker, i);
    }
    for (auto& t : threads) {
        t.join();
    }
    return 0;
}
