#include <iostream>
#include <thread>
#include <atomic>
#include <vector>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <csignal>
#include <sys/select.h>
#include <mutex>  // Added for std::mutex

class InputMonitor {
    std::atomic<bool> running{true};
    struct termios original_termios;
    std::vector<char> input_buffer;
    std::mutex print_mutex;  // Corrected mutex declaration
    std::vector<std::vector<char>>input_data()

    void setNonCanonicalMode() {
        struct termios newt;
        tcgetattr(STDIN_FILENO, &original_termios);
        newt = original_termios;
        newt.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHOK | ECHONL);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    }

    void restoreTerminalMode() {
        tcsetattr(STDIN_FILENO, TCSANOW, &original_termios);
    }

    int getCharNonBlocking() {
        struct timeval tv = {0, 0};
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO, &fds);
        
        if (select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) > 0) {
            return getchar();
        }
        return EOF;
    }

    void safePrint(const std::string& str) {
        std::lock_guard<std::mutex> lock(print_mutex);
        std::cout << str << std::endl;
    }

public:
    void monitorEnterPress() {
    while (running) {
        int ch = getCharNonBlocking();
        if (ch == '\n') {
            // Create a copy of the buffer for thread safety
            std::vector<char> buffer_copy;
            {
                std::lock_guard<std::mutex> lock(print_mutex);
                buffer_copy = input_buffer;
                input_buffer.clear();  // Clear the buffer after copying
            }
            
            // Print each character in the buffer
            for (char x : buffer_copy) {
                safePrint(std::string(1, x));  // Convert char to string
            }
            
            // Indicate Enter was pressed
            safePrint("-- Enter pressed --");
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

    void monitorTextInput() {
        while (running) {
            int ch = getCharNonBlocking();
            
            if (ch != EOF) {
                if (ch == '\n') {
                    if (!input_buffer.empty()) {
                        std::string input(input_buffer.begin(), input_buffer.end());
                        safePrint("Text entered: " + input);
                        input_buffer.clear();
                    }
                } else {
                    input_buffer.push_back(static_cast<char>(ch));
                }
            }
            
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    void start() {
        setNonCanonicalMode();
        std::thread enterThread(&InputMonitor::monitorEnterPress, this);
        std::thread inputThread(&InputMonitor::monitorTextInput, this);

        // Wait for threads to finish
        enterThread.join();
        inputThread.join();
        restoreTerminalMode();
    }

    void stop() {
        running = false;
    }
};

int main() {
    InputMonitor monitor;
    std::cout << "Input monitoring started. Type 'exit' to quit..." << std::endl;
    
    // Start monitoring
    monitor.start();
    
    // Alternative exit mechanism
    std::string exit_cmd;
    while (true) {
        std::cin >> exit_cmd;
        if (exit_cmd == "exit") {
            monitor.stop();
            break;
        }
    }
    
    return 0;
}