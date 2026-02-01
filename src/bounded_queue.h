#include <iostream>
#include <condition_variable>
#include <mutex>

class BoundedQueue
{
public:
    explicit BoundedQueue(size_t cap);
    void Push(int x);
    int Pop();
    void TryPush(int x); // does not block when queue is full
    int TryPop();
    ~BoundedQueue();
    size_t size = 0;

private:
    int *arr; // points to start of dynamically sized array
    size_t cap;
    size_t head = 0;
    std::condition_variable cv;
    std::mutex mtx;
};

// 1) just int types, then generic template