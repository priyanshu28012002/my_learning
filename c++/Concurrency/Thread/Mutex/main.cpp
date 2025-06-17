#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

std::mutex mtx;
int shared_data = 0;

void increment(int id) {
    for(int i = 0; i < 1000; ++i) {
        std::lock_guard<std::mutex> lock(mtx);
        shared_data++;
    }
    std::cout << "Thread " << id << " finished\n";
}

int main() {
    const int num_threads = 10;
    std::vector<std::thread> threads;
    
    for(int i = 0; i < num_threads; ++i) {
        threads.emplace_back(increment, i);
    }
    
    for(auto &t : threads) {
        t.join();
    }
    
    std::cout << "Final shared_data value: " << shared_data << std::endl;
    return 0;
}