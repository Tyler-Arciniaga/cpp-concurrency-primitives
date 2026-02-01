#include "../src/bounded_queue.h"
#include <chrono>
#include <thread>
void ThreadPop(BoundedQueue *bq)
{
    std::this_thread::sleep_for(std::chrono::seconds(4));
    int x = bq->Pop();
    std::cout << "Popped element: " << x << "\n";
}
int main()
{
    BoundedQueue bq(2);

    bq.Push(1);
    bq.Push(2);

    std::cout << bq.size << "\n";

    std::thread t(ThreadPop, &bq);
    bq.Push(3);
    t.join();
}