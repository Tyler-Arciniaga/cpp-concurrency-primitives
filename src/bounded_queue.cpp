#include "bounded_queue.h"

BoundedQueue::BoundedQueue(size_t capacity)
{
    cap = capacity;
    arr = new int[capacity];
}

int BoundedQueue::Size()
{
    return size;
}

void BoundedQueue::Push(int x)
{
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [&]
            { return size < cap; });

    size_t index = (head + size) % cap;
    arr[index] = x;
    size++;
    cv.notify_one();
    // std::cout << "pushed!\n";
}

bool BoundedQueue::TryPush(int x)
{
    bool res = false;
    {
        std::unique_lock<std::mutex> lock(mtx);
        if (size < cap)
        {
            size_t index = (head + size) % cap;
            arr[index] = x;
            size++;
            res = true;
        }
    }

    return res;
}

int BoundedQueue::TryPop()
{
}

int BoundedQueue::Pop()
{
    int x;
    {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [&]
                { return size > 0; });
        x = arr[head];
        head = (head + 1) % cap;
        size--;
        // std::cout << "popped!\n";
    }
    cv.notify_one(); // TODO: enforce ordering of which blocked clients are handled first (client queue)
    return x;
}

BoundedQueue::~BoundedQueue()
{
    delete[] arr;
}