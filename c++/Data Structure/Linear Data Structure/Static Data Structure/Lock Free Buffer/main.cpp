// #include <iostream>
// #include <vector>
// #include <thread>
// #include <chrono>
// #include <atomic> 
// #include <mutex>

// const int buffer_size = 4;

// template<typename T, size_t Size>
// class LockFreeBuffer {
// private:
//     std::vector<T> buffer;
//     std::atomic<size_t> head;
//     std::atomic<size_t> tail;
//     mutable std::mutex buffer_mutex; // For thread-safe buffer access

// public:
//     LockFreeBuffer();
//     ~LockFreeBuffer();

//     bool push(const T& item);
//     bool pop(T& item);
//     bool isEmpty() const;
//     std::vector<T> getBufferSnapshot() const; // Renamed for clarity
// };

// template<typename T, size_t Size>
// LockFreeBuffer<T, Size>::LockFreeBuffer() : 
//     buffer(Size), 
//     head(0), 
//     tail(0)
// {
//     std::cout << "LockFreeBuffer created with size: " << Size << std::endl;
// }

// template<typename T, size_t Size>
// LockFreeBuffer<T, Size>::~LockFreeBuffer() {
//     std::cout << "LockFreeBuffer destroyed." << std::endl;
// }

// template<typename T, size_t Size>
// bool LockFreeBuffer<T, Size>::push(const T& item) {
//     size_t current_head = head.load(std::memory_order_relaxed);
//     size_t next_head = (current_head + 1) % Size;

//     // if (next_head == tail.load(std::memory_order_acquire)) {
//     //     return false; // Buffer is full
//     // }

//     buffer[current_head] = item;
//     head.store(next_head, std::memory_order_release);
//     return true;
// }

// template<typename T, size_t Size>
// bool LockFreeBuffer<T, Size>::pop(T& item) {
//     size_t current_tail = tail.load(std::memory_order_relaxed);
    
//     if (current_tail == head.load(std::memory_order_acquire)) {
//         return false; // Buffer is empty
//     }

//     item = buffer[current_tail];
//     tail.store((current_tail + 1) % Size, std::memory_order_release);
//     return true;
// }

// template<typename T, size_t Size>
// bool LockFreeBuffer<T, Size>::isEmpty() const {
//     return head.load(std::memory_order_acquire) == tail.load(std::memory_order_acquire);
// }

// template<typename T, size_t Size>
// std::vector<T> LockFreeBuffer<T, Size>::getBufferSnapshot() const {
//     std::lock_guard<std::mutex> lock(buffer_mutex);
//     std::vector<T> snapshot;
//     size_t t = tail.load(std::memory_order_acquire);
//     size_t h = head.load(std::memory_order_acquire);
    
//     if (t == h) return snapshot; // Empty buffer
    
//     size_t i = t;
//     while (i != h) {
//         snapshot.push_back(buffer[i]);
//         i = (i + 1) % Size;
//     }
//     return snapshot;
// }

// void producerThread(LockFreeBuffer<int, buffer_size>& buf, std::atomic<bool>& running) {
//     int i = 0;
//     while(running) {
//         if (buf.push(i)) {
//             std::cout << "Producer pushed: " << i << std::endl;
//             i++;
//         } else {
//             i--;
//             std::cout << "Producer waiting (buffer full)" << std::endl;
//         }
//         std::this_thread::sleep_for(std::chrono::milliseconds(500));
//     }
// }

// void printBuffer(LockFreeBuffer<int, buffer_size>& buf, std::atomic<bool>& running) {
//     while(running) {
//         std::this_thread::sleep_for(std::chrono::seconds(1));
//         auto snapshot = buf.getBufferSnapshot();
        
//         std::cout << "Buffer contents: ";
//         for(auto x : snapshot) {
//             std::cout << x << " ";
//         }
//         std::cout << std::endl;
//     }
// }

// int main() {
//     LockFreeBuffer<int, buffer_size> lockFreeBuffer;
//     std::atomic<bool> running{true};

//     std::thread producer(producerThread, std::ref(lockFreeBuffer), std::ref(running));
//     std::thread printer(printBuffer, std::ref(lockFreeBuffer), std::ref(running));

//     // Let it run for 5 seconds
//     std::this_thread::sleep_for(std::chrono::seconds(50));
    
//     // Signal threads to stop
//     running = false;
    
//     // Wait for threads to finish
//     producer.join();
//     printer.join();

//     return 0;
// }

// #include <iostream>
// #include <fstream>
// #include <string>
// #include <queue>
// #include <thread>
// #include <mutex>
// #include <condition_variable>
// #include <vector>
// #include <memory>
// #include <functional>
// #include <atomic>

// template<typename T>
// class ThreadSafeQueue {
// private:
//     std::queue<T> queue;
//     std::mutex mutex;
//     std::condition_variable cond;
//     std::atomic<bool> shutdown_flag{false};
//     size_t max_size;

// public:
//     explicit ThreadSafeQueue(size_t max_size = 10) : max_size(max_size) {}

//     bool push(T item) {
//         std::unique_lock<std::mutex> lock(mutex);
//         cond.wait(lock, [this]() { return queue.size() < max_size || shutdown_flag.load(); });
//         if (shutdown_flag.load()) return false;
//         queue.push(std::move(item));
//         cond.notify_all();
//         return true;
//     }

//     bool pop(T& item) {
//         std::unique_lock<std::mutex> lock(mutex);
//         cond.wait(lock, [this]() { return !queue.empty() || shutdown_flag.load(); });
//         if (queue.empty() && shutdown_flag.load()) return false;
//         if (queue.empty()) return false;
//         item = std::move(queue.front());
//         queue.pop();
//         cond.notify_all();
//         return true;
//     }

//     void shutdown() {
//         shutdown_flag.store(true);
//         cond.notify_all();
//     }

//     bool is_shutdown() const {
//         return shutdown_flag.load();
//     }

//     size_t size() const {
//         std::lock_guard<std::mutex> lock(mutex);
//         return queue.size();
//     }
// };
// class TextProcessor {
// public:
//     static std::string toUpperCase(const std::string& input) {
//         std::string output;
//         output.reserve(input.size());
//         for (char c : input) {
//             output += static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
//         }
//         return output;
//     }
// };

// class FileHandler {
// public:
//     static std::string readChunk(std::ifstream& file, size_t chunk_size) {
//         std::string chunk;
//         chunk.resize(chunk_size);
//         file.read(&chunk[0], chunk_size);
//         chunk.resize(file.gcount());
//         return chunk;
//     }
// };

// class ThreadPool {
// private:
//     std::vector<std::thread> workers;
//     std::atomic<bool> stop{false};

// public:
//     ThreadPool(size_t threads, std::function<void()> func) {
//         workers.reserve(threads);
//         for (size_t i = 0; i < threads; ++i) {
//             workers.emplace_back(func);
//         }
//     }

//     ~ThreadPool() {
//         stop = true;
//         for (auto& worker : workers) {
//             if (worker.joinable()) {
//                 worker.join();
//             }
//         }
//     }
// };

// class ProducerConsumerManager {
// private:
//     ThreadSafeQueue<std::string> buffer;
//     std::ifstream input_file;
//     std::ofstream output_file;
//     std::atomic<bool> file_read_complete{false};
//     const size_t chunk_size = 4096; // 4KB chunks

// public:
//     ProducerConsumerManager(const std::string& input_path, const std::string& output_path, size_t buffer_size)
//         : buffer(buffer_size) {
//         input_file.open(input_path);
//         if (!input_file.is_open()) {
//             throw std::runtime_error("Failed to open input file");
//         }
//         output_file.open(output_path);
//         if (!output_file.is_open()) {
//             throw std::runtime_error("Failed to open output file");
//         }
//     }

//     ~ProducerConsumerManager() {
//         buffer.shutdown();
//         if (input_file.is_open()) input_file.close();
//         if (output_file.is_open()) output_file.close();
//     }

//     void producer() {
//         while (!file_read_complete.load() && !buffer.shutdown_flag.load()) {
//             auto chunk = FileHandler::readChunk(input_file, chunk_size);
//             if (chunk.empty()) {
//                 file_read_complete.store(true);
//                 break;
//             }
//             auto processed_chunk = TextProcessor::toUpperCase(chunk);
//             buffer.push(processed_chunk);
//         }
//     }

//     void consumer() {
//         std::string chunk;
//         while (true) {
//             if (!buffer.pop(chunk)) break;
//             output_file << chunk;
//             output_file.flush();
//         }
//     }

//     void run(size_t producer_threads, size_t consumer_threads) {
//         // Create producer threads
//         ThreadPool producers(producer_threads, [this]() { this->producer(); });
        
//         // Create consumer threads
//         ThreadPool consumers(consumer_threads, [this]() { this->consumer(); });
        
//         // ThreadPools will automatically join in their destructors
//     }
// };

// int main() {
//     try {
//         const std::string input_file = "input.txt";
//         const std::string output_file = "output.txt";
//         const size_t buffer_size = 20; // Number of chunks in buffer
//         const size_t producer_threads = 4;
//         const size_t consumer_threads = 4;

//         ProducerConsumerManager manager(input_file, output_file, buffer_size);
//         manager.run(producer_threads, consumer_threads);

//         std::cout << "File processing completed successfully." << std::endl;
//     } catch (const std::exception& e) {
//         std::cerr << "Error: " << e.what() << std::endl;
//         return 1;
//     }

//     return 0;
// }

#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <mutex>
#include <thread>
#include <vector>

// Thread-safe buffer
std::queue<std::string> buffer;
std::mutex buffer_mutex;
std::condition_variable buffer_cv;
bool done_reading = false;

// Producer: Reads file → processes → stores in buffer
void producer(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line)) {
        // Convert to uppercase (processing)
        for (char& c : line) c = toupper(c);

        // Store in buffer (thread-safe)
        {
            std::lock_guard<std::mutex> lock(buffer_mutex);
            buffer.push(line);
            buffer_cv.notify_one(); // Notify consumers
        }
    }

    // Signal end of file
    {
        std::lock_guard<std::ex::mutex> lock(buffer_mutex);
        done_reading = true;
        buffer_cv.notify_all();
    }
}

// Consumer: Takes from buffer → saves to memory (output file)
void consumer(const std::string& out_filename) {
    std::ofstream out_file(out_filename);

    while (true) {
        std::string line;
        {
            std::unique_lock<std::mutex> lock(buffer_mutex);
            buffer_cv.wait(lock, [] { return !buffer.empty() || done_reading; });

            if (buffer.empty() && done_reading) break; // Exit if done

            line = buffer.front();
            buffer.pop();
        }

        // Save to memory (output file)
        out_file << line << "\n";
    }
}

int main() {
    const std::string input_file = "input.txt";
    const std::string output_file = "output.txt";

    // Start 1 producer and 1 consumer (simplified)
    std::thread producer_thread(producer, input_file);
    std::thread consumer_thread(consumer, output_file);

    producer_thread.join();
    consumer_thread.join();

    std::cout << "Processing complete. Output saved to " << output_file << std::endl;
    return 0;
}