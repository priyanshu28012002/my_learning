// #include <iostream>
// #include <atomic>
// #include <vector>
// #include <thread>
// #include <unistd.h>
// #include <termios.h>

// // Lock-free circular buffer
// template <typename T, size_t Size>
// class LockFreeBuffer {
//     std::array<T, Size> buffer;
//     std::atomic<size_t> head{0};
//     std::atomic<size_t> tail{0};

// public:
//     bool push(const T& item) {
//         size_t current_tail = tail.load(std::memory_order_relaxed);
//         size_t next_tail = (current_tail + 1) % Size;

//         if (next_tail == head.load(std::memory_order_acquire)) {
//             return false; // Buffer full
//         }

//         buffer[current_tail] = item;
//         tail.store(next_tail, std::memory_order_release);
//         return true;
//     }

//     bool pop(T& item) {
//         size_t current_head = head.load(std::memory_order_relaxed);

//         if (current_head == tail.load(std::memory_order_acquire)) {
//             return false; // Buffer empty
//         }

//         item = buffer[current_head];
//         head.store((current_head + 1) % Size, std::memory_order_release);
//         return true;
//     }

//     bool empty() const {
//         return head.load(std::memory_order_relaxed) == 
//                tail.load(std::memory_order_relaxed);
//     }
// };

// // Non-blocking keyboard input
// void setNonBlockingInput(bool enable) {
//     static struct termios oldt, newt;
//     if (enable) {
//         tcgetattr(STDIN_FILENO, &oldt);
//         newt = oldt;
//         newt.c_lflag &= ~(ICANON | ECHO);
//         tcsetattr(STDIN_FILENO, TCSANOW, &newt);
//     } else {
//         tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
//     }
// }

// int main() {
//     constexpr size_t buffer_size = 1024;
//     LockFreeBuffer<char, buffer_size> char_buffer;
//     std::string current_sentence;
    
//     setNonBlockingInput(true);

//     // Producer thread - reads keyboard input
//     std::thread producer([&]() {
//         while (true) {
//             char ch = getchar();
//             if (ch != EOF) {
//                 if (!char_buffer.push(ch)) {
//                     std::cerr << "Buffer full, dropping input!\n";
//                 }
//             }
//             std::this_thread::sleep_for(std::chrono::milliseconds(10));
//         }
//     });

//     // Consumer - processes input
//     while (true) {
//         char ch;
//         if (char_buffer.pop(ch)) {
//             if (ch == '\n') {
//                 std::cout << "\nComplete sentence: " << current_sentence << std::endl;
//                 current_sentence.clear();
//             } else {
//                 current_sentence += ch;
//                 std::cout << ch << std::flush; // Echo character
//             }
//         }
        
//         // Exit condition
//         if (!current_sentence.empty() && 
//             current_sentence.find("exit") != std::string::npos) {
//             break;
//         }
        
//         std::this_thread::sleep_for(std::chrono::milliseconds(10));
//     }

//     producer.detach();
//     setNonBlockingInput(false);
//     std::cout << "Program exited cleanly.\n";
//     return 0;
// }


#include <iostream>
#include <fstream>
#include <string>
#include <atomic>
#include <array>
#include <thread>
#include <unistd.h>
#include <termios.h>

// =============================================
// Lock-Free Circular Buffer for Keyboard Input
// =============================================
template <typename T, size_t Size>
class LockFreeBuffer {
    std::array<T, Size> buffer;      // Storage array
    std::atomic<size_t> head{0};     // Read position (atomic for thread safety)
    std::atomic<size_t> tail{0};     // Write position (atomic for thread safety)

public:
    // Push a character into the buffer (thread-safe)
    bool push(const T& item) {
        size_t current_tail = tail.load(std::memory_order_relaxed);
        size_t next_tail = (current_tail + 1) % Size;

        if (next_tail == head.load(std::memory_order_acquire)) {
            return false; // Buffer full
        }

        buffer[current_tail] = item;
        tail.store(next_tail, std::memory_order_release);
        return true;
    }

    // Pop a character from the buffer (thread-safe)
    bool pop(T& item) {
        size_t current_head = head.load(std::memory_order_relaxed);

        if (current_head == tail.load(std::memory_order_acquire)) {
            return false; // Buffer empty
        }

        item = buffer[current_head];
        head.store((current_head + 1) % Size, std::memory_order_release);
        return true;
    }
};

// =============================================
// Terminal Configuration Utilities
// =============================================
void setNonBlockingInput(bool enable) {
    static struct termios oldt, newt;
    if (enable) {
        tcgetattr(STDIN_FILENO, &oldt);  // Save current terminal settings
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO); // Disable line buffering & echo
        tcsetattr(STDIN_FILENO, TCSANOW, &newt); // Apply new settings
    } else {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // Restore original settings
    }
}

// =============================================
// File Handling Functions
// =============================================
std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) return "";
    
    std::string content((std::istreambuf_iterator<char>(file)), 
                      std::istreambuf_iterator<char>());
    return content;
}

void writeFile(const std::string& filename, const std::string& content) {
    std::ofstream file(filename);
    file << content;
}

// =============================================
// Main Program
// =============================================
int main() {
    constexpr size_t buffer_size = 1024;
    LockFreeBuffer<char, buffer_size> char_buffer;
    std::string current_content;
    std::string filename;

    // Ask user for filename
    std::cout << "Enter filename (new or existing): ";
    std::cin >> filename;

    // Load existing file content if available
    current_content = readFile(filename);
    if (!current_content.empty()) {
        std::cout << "\nExisting file content:\n";
        std::cout << "======================\n";
        std::cout << current_content << "\n";
        std::cout << "======================\n";
        std::cout << "Start editing (Ctrl+D to save & exit)\n";
    } else {
        std::cout << "\nCreating new file. Start typing (Ctrl+D to save & exit)\n";
    }

    // Set terminal to non-blocking mode
    setNonBlockingInput(true);

    // Producer thread - captures keyboard input
    std::thread producer([&]() {
        while (true) {
            char ch = getchar();
            if (ch != EOF) {
                if (!char_buffer.push(ch)) {
                    std::cerr << "Buffer full!\n";
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    });

    // Consumer - processes input and builds content
    bool running = true;
    while (running) {
        char ch;
        if (char_buffer.pop(ch)) {
            if (ch == 4) { // ASCII 4 = Ctrl+D (save and exit)
                running = false;
            } else {
                current_content += ch;
                std::cout << ch << std::flush; // Echo character
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    // Cleanup
    producer.detach();
    setNonBlockingInput(false);

    // Save file
    writeFile(filename, current_content);
    std::cout << "\nFile saved successfully. Exiting...\n";

    return 0;
}