// #include <iostream>
// #include <thread>
// #include <atomic>
// #include <vector>
// #include <chrono>

// template<typename T, size_t Size>
// class LockFreeRingBuffer {
//     static_assert((Size & (Size - 1)) == 0, "Size must be a power of 2");

//     std::vector<T> buffer;
//     std::atomic<size_t> head;
//     std::atomic<size_t> tail;

// public:
//     LockFreeRingBuffer() : buffer(Size), head(0), tail(0) {}

//     bool push(const T& item) {
//         size_t h = head.load(std::memory_order_relaxed);
//         size_t next = (h + 1) & (Size - 1);

//         if (next == tail.load(std::memory_order_acquire)) {
//             return false; // buffer full
//         }

//         buffer[h] = item;
//         head.store(next, std::memory_order_release);
//         return true;
//     }

//     bool pop(T& item) {
//         size_t t = tail.load(std::memory_order_relaxed);

//         if (t == head.load(std::memory_order_acquire)) {
//             return false; // buffer empty
//         }

//         item = buffer[t];
//         tail.store((t + 1) & (Size - 1), std::memory_order_release);
//         return true;
//     }
// };

// int main() {
//     LockFreeRingBuffer<int, 2> ringBuffer;

//     // Producer thread
//     std::thread producer([&]() {
//         for (int i = 1; i <= 10; ++i) {
//             while (!ringBuffer.push(i)) {
//                 std::this_thread::yield(); // wait if buffer is full
//             }
//             std::cout << "Produced: " << i << std::endl;
//             std::this_thread::sleep_for(std::chrono::milliseconds(100));
//         }
//     });

//     // Consumer thread
//     std::thread consumer([&]() {
//         int value;
//         for (int i = 1; i <= 10; ++i) {
//             while (!ringBuffer.pop(value)) {
//                 std::this_thread::yield(); // wait if buffer is empty
//             }
//             std::cout << "Consumed: " << value << std::endl;
//             std::this_thread::sleep_for(std::chrono::seconds(1));
//         }
//     });

//     producer.join();
//     consumer.join();

//     return 0;
// }

#include <iostream>
#include <thread>
#include <atomic>
#include <vector>
#include <string>
#include <chrono>

template<typename T, size_t Size>
class LockFreeRingBuffer {
    static_assert((Size & (Size - 1)) == 0, "Size must be a power of 2");

    std::vector<T> buffer;
    std::atomic<size_t> head;
    std::atomic<size_t> tail;

public:
    LockFreeRingBuffer() : buffer(Size), head(0), tail(0) {}

    bool push(const T& item) {
        size_t h = head.load(std::memory_order_relaxed);
        size_t next = (h + 1) & (Size - 1);

        if (next == tail.load(std::memory_order_acquire)) {
            return false; // buffer full
        }

        buffer[h] = item;
        head.store(next, std::memory_order_release);
        return true;
    }

    bool pop(T& item) {
        size_t t = tail.load(std::memory_order_relaxed);

        if (t == head.load(std::memory_order_acquire)) {
            return false; // buffer empty
        }

        item = buffer[t];
        tail.store((t + 1) & (Size - 1), std::memory_order_release);
        return true;
    }
};

int main() {
    LockFreeRingBuffer<std::string, 2> mangoBuffer;

    

    // Producer thread
    std::thread producer([&]() {
        while(true){
        std::string;
        std::cin>>intput;
            while (!mangoBuffer.push(intput)) {
                std::this_thread::yield(); // wait if buffer is full
            }
            std::cout << "[Producer] " << intput << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
    });

    // Consumer thread
    std::thread consumer([&]() {
        std::string mango;
        for (size_t i = 0; i < mangoTypes.size(); ++i) {
            while (!mangoBuffer.pop(mango)) {
                std::this_thread::yield(); // wait if buffer is empty
            }
            std::cout << "[Consumer] Got: " << mango << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
    });

    producer.join();
    consumer.join();

    return 0;
}
