#include <iostream>
#include <condition_variable>
#include <mutex>
#include <optional>
#include <vector>

template <typename T>

class BoundedQueue
{
public:
    explicit BoundedQueue(size_t cap);
    void Push(T x);
    T Pop();
    bool TryPush(T x); // does not block when queue is full
    std::optional<T> TryPop();
    size_t Size() const;
    // ~BoundedQueue();

private:
    std::vector<T> vec;
    // int *arr; // points to start of dynamically sized array
    size_t size = 0;
    size_t cap;
    size_t head = 0;
    std::condition_variable cv_not_empty;
    std::condition_variable cv_not_full;
    std::mutex mtx;
};

template <typename T>
BoundedQueue<T>::BoundedQueue(size_t capacity)
{
    cap = capacity;
    vec.resize(capacity);
}

template <typename T>
size_t BoundedQueue<T>::Size() const
{
    return size;
}

template <typename T>
void BoundedQueue<T>::Push(T x)
{
    std::unique_lock<std::mutex> lock(mtx);
    cv_not_full.wait(lock, [&]
                     { return size < cap; });

    size_t index = (head + size) % cap;
    vec[index] = std::move(x);
    size++;
    cv_not_empty.notify_one();
    std::cout << "pushed!\n";
}

template <typename T>
bool BoundedQueue<T>::TryPush(T x)
{
    bool res = false;
    {
        std::unique_lock<std::mutex> lock(mtx);
        if (size < cap)
        {
            size_t index = (head + size) % cap;
            vec[index] = std::move(x);
            size++;
            res = true;
            cv_not_empty.notify_one();
        }
    }

    return res;
}

template <typename T>
std::optional<T> BoundedQueue<T>::TryPop()
{
    std::unique_lock<std::mutex> lock(mtx);
    if (size > 0)
    {
        T x = std::move(vec[head]);
        head = (head + 1) % cap;
        size--;
        cv_not_full.notify_one();
        return x;
    }
    return std::nullopt;
}

template <typename T>
T BoundedQueue<T>::Pop()
{
    T x;
    {
        std::unique_lock<std::mutex> lock(mtx);
        cv_not_empty.wait(lock, [&]
                          { return size > 0; });
        x = std::move(vec[head]);
        head = (head + 1) % cap;
        size--;
        std::cout << "popped!\n";
    }
    cv_not_full.notify_one(); // TODO: enforce ordering of which blocked clients are handled first (client queue)
    return x;
}