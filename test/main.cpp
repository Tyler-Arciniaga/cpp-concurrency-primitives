#include "../src/sushi_pool.h"

void one_sec_task()
{
    std::cout << "this task takes 1 second...\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
}
void three_sec_task()
{
    std::cout << "this task takes 3 seconds...\n";
    std::this_thread::sleep_for(std::chrono::seconds(3));
}
void five_sec_task()
{
    std::cout << "this task takes 5 seconds...\n";
    std::this_thread::sleep_for(std::chrono::seconds(5));
}

int main()
{
    void (*one_sec_task_ptr)() = &one_sec_task;
    void (*three_sec_task_ptr)() = &three_sec_task;
    void (*five_sec_task_ptr)() = &five_sec_task;

    ThreadPool pool(3);
    pool.submit(three_sec_task);
    pool.submit(one_sec_task);
    pool.submit(five_sec_task);

    pool.shutdown();
    return 0;
}