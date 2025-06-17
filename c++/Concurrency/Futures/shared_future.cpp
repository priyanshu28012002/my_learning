// The class template std::shared_future provides a mechanism to access the result of asynchronous operations,
// similar to std::future, except that multiple threads are allowed to wait for the same shared state. Unlike std::future,
// which is only moveable (so only one instance can refer to any particular asynchronous result),
// std::shared_future is copyable and multiple shared future objects may refer to the same shared state.
// Access to the same shared state from multiple threads is safe if each thread does it through its own copy of a shared_future object.
#include <iostream>
#include <future>
#include <thread>
#include <vector>

void worker(std::shared_future<int> shared_fut, int id)
{
    
    
    int result ;
    try

    {
        if (shared_fut.wait_for(std::chrono::seconds(id)) == std::future_status::ready)
        {
            result = shared_fut.get();
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << "\n";
    }
    std::this_thread::sleep_for(std::chrono::seconds(id));

    std::cout << "Thread " << id << " got: " << result << "\n";
}

void setVal(std::promise<int> &prom)
{ // Take promise by reference
    std::this_thread::sleep_for(std::chrono::seconds(1));
    prom.set_value(100);
}

int main()
{
    std::promise<int> prom;
    std::shared_future<int> shared_fut = prom.get_future().share();

    std::vector<std::thread> threads;
    for (int i = 0; i < 5; ++i)
    {
        threads.emplace_back(worker, shared_fut, i);
    }

    std::thread setValThread(setVal, std::ref(prom)); // Pass by reference

    setValThread.join();
    for (auto &t : threads)
        t.join();

    return 0;
}