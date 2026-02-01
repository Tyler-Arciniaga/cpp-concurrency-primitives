#include "thread_pool.h"

ThreadPool::ThreadPool(size_t numWorkers)
{
    std::cout << "starting pool...\n";

    for (int i = 0; i < numWorkers; i++)
    {
        workers.emplace_back([this, i]
                             { ExecuteJob(i); });
    }
}

void ThreadPool::ExecuteJob(int id)
{
    while (!stop)
    {
        std::function<void()> job;
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            cv.wait(lock, [&]
                    { return stop || !job_queue.empty(); });
            if (stop && job_queue.empty())
            {
                return;
            }
            job = std::move(job_queue.front());
            job_queue.pop();
        } // releases lock after leaving local scope

        std::cout << "thread " << id << " is now working on a job...\n";
        job();
        std::cout << "thread " << id << " has now finished on a job!\n";
    }
}

void ThreadPool::submit(std::function<void()> f)
{
    {
        std::unique_lock<std::mutex> lock(queue_mutex); // automatically unlocks when the unique_lock goes out of scope
        job_queue.push(f);
    }
    cv.notify_one();
}

void ThreadPool::shutdown()
{
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }
    cv.notify_all();

    for (std::thread &t : workers)
    {
        if (t.joinable())
        {
            t.join();
        }
    }
}

ThreadPool::~ThreadPool()
{
    shutdown();
}