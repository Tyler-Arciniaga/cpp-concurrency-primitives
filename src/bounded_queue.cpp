#include "bounded_queue.h"

BoundedQueue::BoundedQueue(size_t capacity)
{
    cap = capacity;
    arr = new int[capacity];
}

void BoundedQueue::Push(int x)
{
    size_t index = (head + size) % cap;

    std::unique_lock<std::mutex> lock(mtx);
    if (size == cap)
    {
        cv.wait(lock, [&]
                { return size < cap; });
        head = (head + 1) % cap;
    }
    else
    {
        size++;
    }
    arr[index] = x;
    // std::cout << "pushed!\n";
}

int BoundedQueue::Pop()
{
    int x;
    {
        std::unique_lock<std::mutex> lock(mtx);
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