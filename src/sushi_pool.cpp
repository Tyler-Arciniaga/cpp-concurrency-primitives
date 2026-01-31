#include "sushi_pool.h"

ThreadPool::ThreadPool(size_t numWorkers)
{
    std::cout << "starting pool...\n";

    for (int i = 0; i < numWorkers; i++)
    {
        std::thread t(ExecuteJob, i, this);
        t.detach();
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

void ThreadPool::ExecuteJob(int id, ThreadPool *pool)
{
    while (true)
    {
        std::unique_lock<std::mutex> lock(pool->queue_mutex);
        pool->cv.wait(lock, [&]
                      { return !pool->job_queue.empty(); });
        std::function<void()> job = pool->job_queue.front();
        pool->job_queue.pop();
        lock.unlock();
        std::cout << "thread " << id << " is now working on a job...\n";
        job();
        std::cout << "thread " << id << " has now finished on a job!\n";
    }
}

void ThreadPool::submit(std::function<void()> f)
{
    std::unique_lock<std::mutex> lock(queue_mutex); // automatically unlocks when the unique_lock goes out of scope
    job_queue.push(f);
    cv.notify_one();
}