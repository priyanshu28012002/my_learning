#include <chrono>
#include <iostream>
#include <string_view>
#include <thread>

using namespace std::chrono_literals;

// Utility function to print stop_token status
void print(std::string_view name, const std::stop_token& token)
{
    std::cout << name << ": stop_possible = " << token.stop_possible()
              << ", stop_requested = " << token.stop_requested() << '\n';
}

// Worker that listens for stop requests
void finite_sleepy(std::stop_token stoken)
{
    for (int i = 10; i; --i)
    {
        std::this_thread::sleep_for(300ms);
        if (stoken.stop_requested())
        {
            std::cout << "  Sleepy worker is requested to stop\n";
            return;
        }
        std::cout << "  Sleepy worker goes back to sleep\n";
    }
}

// Worker that ignores stop requests (for comparison)
void infinite_sleepy()
{
    for (int i = 5; i; --i)
    {
        std::this_thread::sleep_for(300ms);
        std::cout << "  Run as long as we want\n";
    }
}

int main()
{
    std::cout << std::boolalpha;

    // Start worker that listens to stop_token
    std::jthread stop_worker(finite_sleepy);

    // Start worker that doesn’t respond to stop_token
    std::jthread inf_worker(infinite_sleepy);

    // Get stop tokens
    std::stop_token stop_token = stop_worker.get_stop_token();
    std::stop_token inf_token = inf_worker.get_stop_token();

    // Print token status
    print("stop_token", stop_token);
    print("inf_token ", inf_token);

    std::cout << "\nRequest and join stop_worker:\n";
    stop_worker.request_stop(); // Ask the first thread to stop
    stop_worker.join();         // Wait for it to finish

    std::cout << "\nRequest and join inf_worker:\n";
    inf_worker.request_stop();  // Has no effect; worker ignores it
    inf_worker.join();          // Wait for it to finish

    std::cout << "\nAll done.\n";
}
