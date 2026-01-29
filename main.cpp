#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <string>

std::mutex m;
std::condition_variable cv;

void ExecuteTask(int id)
{
    std::unique_lock<std::mutex> lock(m);
    cv.wait(lock);
    std::cout << "I am a thread " << id << " and I am executing this task...\n";
}
int main()
{
    // 1) Instantiate single worker thread
    std::thread t(ExecuteTask, 67);
    // 2) sleep the worker thread
    // this is handled by condition variable

    // 3) wake up the worker thread once theres some job in the queue

    // some blocking call
    std::string s;
    std::cin >> s;

    // wakes up thread
    cv.notify_one();

    t.join(); // wait for thread to finish
}