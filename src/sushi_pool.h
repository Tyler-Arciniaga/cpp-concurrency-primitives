#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <queue>
#include <chrono>

class ThreadPool
{
public:
    explicit ThreadPool(size_t numWorkers);
    std::queue<std::function<void()>> job_queue;
    std::mutex queue_mutex;
    std::condition_variable cv;

    void submit(std::function<void()> f);
    // void shutdown();
    // ~ThreadPool();

private:
    static void ExecuteJob(int id, ThreadPool *pool);
};