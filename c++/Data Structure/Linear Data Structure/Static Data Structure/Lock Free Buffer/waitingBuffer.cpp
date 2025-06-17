#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <atomic> 
#include <mutex>
#include <condition_variable>

const int buffer_size = 4;

template<typename T, size_t Size>
class LockFreeBuffer {
private:
    std::vector<T> buffer;
    std::atomic<size_t> head;
    std::atomic<size_t> tail;

public:
    LockFreeBuffer();
    ~LockFreeBuffer();

    bool push(const T& item);
    bool pop(T& item);
    bool isEmpty() const;
};

template<typename T, size_t Size>
LockFreeBuffer<T, Size>::LockFreeBuffer() : 
    buffer(Size), 
    head(0), 
    tail(0)
{
    std::cout << "LockFreeBuffer created with size: " << Size << std::endl;
}

template<typename T, size_t Size>
LockFreeBuffer<T, Size>::~LockFreeBuffer() {
    std::cout << "LockFreeBuffer destroyed." << std::endl;
}

template<typename T, size_t Size>
bool LockFreeBuffer<T, Size>::push(const T& item) {
    size_t current_head = head.load(std::memory_order_relaxed);
    size_t next_head = (current_head + 1) % Size;

    if (next_head == tail.load(std::memory_order_acquire)) {
        return false; // Buffer is full
    }

    buffer[current_head] = item;
    head.store(next_head, std::memory_order_release);
    return true;
}

template<typename T, size_t Size>
bool LockFreeBuffer<T, Size>::pop(T& item) {
    size_t current_tail = tail.load(std::memory_order_relaxed);
    
    if (current_tail == head.load(std::memory_order_acquire)) {
        return false; // Buffer is empty
    }

    item = buffer[current_tail];
    tail.store((current_tail + 1) % Size, std::memory_order_release);
    return true;
}

template<typename T, size_t Size>
bool LockFreeBuffer<T, Size>::isEmpty() const {
    return head.load(std::memory_order_acquire) == tail.load(std::memory_order_acquire);
}

void producerThread(LockFreeBuffer<int, buffer_size>& buf) {
    int i = 0;
    while(true) {
        if (buf.push(i)) {
            std::cout << "Producer pushed: " << i << std::endl;
            i++;
        } else {
            std::cout << "Producer waiting (buffer full)" << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void consumerThread(LockFreeBuffer<int, buffer_size>& buf, int max_items) {
    int val;
    int popped_count = 0;
    while(popped_count < max_items) {
        if (buf.pop(val)) {
            std::cout << "Consumer popped: " << val << std::endl;
            popped_count++;
        } else {
            std::cout << "Consumer waiting (buffer empty)" << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::cout << "Consumer finished after popping " << max_items << " items" << std::endl;
}

int main() {
    LockFreeBuffer<int, buffer_size> lockFreeBuffer;
    const int items_to_consume = 100;

    std::thread producer(producerThread, std::ref(lockFreeBuffer));
    std::thread consumer(consumerThread, std::ref(lockFreeBuffer), items_to_consume);

    consumer.join();
    
    producer.detach();

    return 0;
}