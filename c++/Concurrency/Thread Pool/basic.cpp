#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <chrono>

class ThreadPool {
private:
    int m_maxThread;
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;

    std::mutex queueMutex;
    std::condition_variable condition;
    bool stop;

public:
    ThreadPool(int i) : m_maxThread(i), stop(false) {
        std::cout << "Constructor is called" << std::endl;
        for (int i = 0; i < m_maxThread; ++i) {
            workers.emplace_back([this] {
                while(true){
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(queueMutex);
                        condition.wait(lock, [this] { return stop || !tasks.empty(); });
                        if (stop && tasks.empty())
                            return;
                        task = std::move(tasks.front());
                        tasks.pop();
                    }
                    task();
                }
            });
        }
    }

    ~ThreadPool() {
        std::cout << "Destructor is called" << std::endl;
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            stop = true;
        }
        condition.notify_all();
        for (std::thread &worker : workers) {
            worker.join();
        }
    }

    template <class F>
    void enqueue(F&& task) {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            tasks.emplace(std::forward<F>(task));
        }
        condition.notify_one();
    }
};

int main() {
    std::cout << "Thread Pool" << std::endl;

    ThreadPool tp(2);

    for (int i = 0; i < 8; i++) {
        tp.enqueue([i] {
            std::cout << "Task " << i << " is running on thread " 
                      << std::this_thread::get_id() << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(5));
        });
    }

    std::this_thread::sleep_for(std::chrono::seconds(5)); 

    return 0;
}
