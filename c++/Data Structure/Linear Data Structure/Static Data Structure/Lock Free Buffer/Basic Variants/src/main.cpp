#include "threadPool.cpp"
#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>
#include <atomic>

std::mutex inputMutex;
int inputValue = 0;


class LockFreeBffer
{
private:
    /* data */
public:
    LockFreeBffer(/* args */);
    ~LockFreeBffer();
};

LockFreeBffer::LockFreeBffer(/* args */)
{
}

LockFreeBffer::~LockFreeBffer()
{
}


void inputFunction() {
    int val = 0;
    while (true) {
        {
            std::lock_guard<std::mutex> lock(inputMutex);
            inputValue = val++;
        }
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}

void printFunction() {
    while (true) {
        {
            // std::lock_guard<std::mutex> lock(inputMutex);
            std::cout << "Current input: " << inputValue << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main() {
    std::cout << "Thread Pool" << std::endl;
    ThreadPool tp(5);  // Make sure your threadPool.cpp defines ThreadPool properly

    tp.enqueue(printFunction);
    tp.enqueue(inputFunction);

    std::this_thread::sleep_for(std::chrono::seconds(30)); // Let threads run for some time
    return 0;
}
