// Threads Demonstration program written by Kevin Nguyen
// OS Concepts

#include <thread>
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include <string.h>
#include <future>

using namespace std;

// sumOf function with return int for future thread
int sumOf(int num)
{
    int result = 0;
    for (int x = num; x < (num + 100); x++)
    {
        result += x;
    }
    return result;
}

// sumOf function for void thread
void voidSumOf(int num)
{
    int result = sumOf(num);
    cout << "The final result is: " << result << endl;
}

int main()
{
    string input;
    cout << "Enter num: ";
    cin >> input; // Too lazy to check input

    // Thread 1 - Standard Threading to a void function
    cout << "Executing t1..." << endl;
    thread t1(voidSumOf, stoi(input)); // Create the thread and run the function sumOf
    t1.join();                         // Wait for t1 to complete!
    cout << "t1 completed." << endl;

    // Thread 2 - Asynchronous Thread with return value by future.
    cout << "\nExecuting t2 using future..." << endl;
    auto future = async(sumOf, stoi(input)); // Execute the thread with future return & using return function (sumOf)
    cout << "t2 completed. Getting future result... ";
    int result = future.get(); // Grab that result from future thread
    cout << result << endl;

    return 0;
}
