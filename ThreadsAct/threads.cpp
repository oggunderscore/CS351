// Threads Demonstration program written by Kevin Nguyen
// OS Concepts

#include <thread>
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include <string.h>
#include <future>
#include <mutex>

using namespace std;

int result = 0;
mutex mtx;

// Runner function
void sumOf()
{
    result = 0;
    for (int x = 0; x <= 1000; x++)
    {
        result += x;
    }
    cout << "Result: " << result << endl;
}

int main()
{

    mtx.lock();
    // Thread 1 - Standard Threading to a void function
    cout << "Executing t1..." << endl;
    thread t1(sumOf); // Create the thread and run the function sumOf
    // Thread 2
    cout << "Executing t2..." << endl;
    thread t2(sumOf); // Create the thread and run the function sumOf

    t1.join(); // Wait for t1 to complete!
    cout << "t1 completed." << endl;
    t2.join(); // Wait for t2 to complete!
    cout << "t2 completed." << endl;

    cout << "Final Result Int: " << result << endl
         << "\n";

    // if (result != 5050)
    // {
    //     cout << "Alternate Result Int: " << result << endl;
    // }
    mtx.unlock();

    return 0;
}
