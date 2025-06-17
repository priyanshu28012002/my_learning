#include <iostream>
#include <fstream>
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

using namespace std;

int main() {
    string filename = "data.txt";
    int value = 0;

    while(true){
        // Read value from file
    ifstream infile(filename);
    if (infile.is_open()) {
        infile >> value;
        infile.close();
    } else {
        cout << "File not found. Starting with 0.\n";
    }

    // Increment value
    value++;

    // Write the new value back to the file
    ofstream outfile(filename);
    if (outfile.is_open()) {
        outfile << value;
        outfile.close();
    } else {
        cerr << "Failed to open file for writing.\n";
        return 1;
    }
            std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
