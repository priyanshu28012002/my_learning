// #include <chrono>
// #include <condition_variable>
// #include <iostream>
// #include <mutex>
// #include <string_view>
// #include <thread>
 
// using namespace std::chrono_literals;
 
// int main()
// {
//     std::cout << std::boolalpha;
//     auto print = [](std::string_view name, const std::stop_source& source)
//     {
//         std::cout << name << ": stop_possible = " << source.stop_possible();
//         std::cout << ", stop_requested = " << source.stop_requested() << '\n';
//     };
 
//     // A worker thread
//     auto worker = std::jthread([](std::stop_token stoken)
//     {
//         for (int i = 10; i; --i)
//         {
//             std::this_thread::sleep_for(300ms);
//             if (stoken.stop_requested())
//             {
//                 std::cout << "  Sleepy worker is requested to stop\n";
//                 return;
//             }
//             std::cout << "  Sleepy worker goes back to sleep\n";
//         }
//     });
 
//     std::stop_source stop_source = worker.get_stop_source();
//     print("stop_source", stop_source);
 
//     std::cout << "\nPass source to other thread:\n";
//     auto stopper = std::thread(
//         [](std::stop_source source)
//         {
//             std::this_thread::sleep_for(500ms);
//             std::cout << "Request stop for worker via source\n";
//             source.request_stop();
//         },
//         stop_source);
//     stopper.join();
//     std::this_thread::sleep_for(200ms);
//     std::cout << '\n';
 
//     print("stop_source", stop_source);
// }


#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <stop_token>

using namespace std::chrono_literals;

void print_tables(std::stop_token stopToken) {
    std::stop_token token = std::this_thread::get_stop_token();

    for (int table = 2; table <= 10; ++table) {
        for (int i = 1; i <= 10; ++i) {
            if (stopToken.stop_requested() || token.stop_requested()) {
                std::cout << "Table printing was requested to stop.\n";
                return;
            }
            std::cout << table << " x " << i << " = " << table * i << std::endl;
            std::this_thread::sleep_for(200ms); // Slow down the printing
        }
        std::cout << "-----------------------------\n";
    }
    std::cout << "Finished printing all tables.\n";
}

void stop_after_delay(std::stop_source stopSource) {
    std::this_thread::sleep_for(2s); // Delay before stopping
    std::cout << "Requesting stop from stopper thread.\n";
    stopSource.request_stop();
}

int main() {
    // Start the printing thread
    std::jthread printer_thread(print_tables);
    
    // Get the stop_source from jthread
    std::stop_source stopSource = printer_thread.get_stop_source();

    // Start a separate thread to stop the printer
    std::thread stopper_thread(stop_after_delay, stopSource);

    stopper_thread.join(); // Wait for the stopper to finish
    return 0;
}

