#include "threadpool.hpp"

int main()
{
    ThreadPool pool;

    // Submit some tasks
    auto future1 = pool.AddTask(TaskWrapper([](int a, int b) { return a + b; }, 5, 3));
    auto future2 = pool.AddTask(TaskWrapper([](float r) { return 3.14f * r * r; }, 4));

    // Get the results
    std::cout << "Result 1: " << std::any_cast<int>(future1.get()) << std::endl;
    std::cout << "Result 2: " << std::any_cast<float>(future2.get()) << std::endl;

    return 0;
}