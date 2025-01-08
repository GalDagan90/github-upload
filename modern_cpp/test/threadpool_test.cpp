#include "threadpool.hpp"

int main()
{
    // Create a thread pool
    ThreadPool threadPool;
    std::vector<std::future<std::any>> futuresVec(100);

    for (int i = 0; i < 10; ++i) 
    {
        futuresVec[i] = 
        threadPool.AddTask(TaskWrapper([i]()
                        {
                            std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Simulate work
                            std::cout << "completed by thread: " << std::this_thread::get_id() << "\n";
                            return i * i;
                        }));
    }

    // Get the results
    std::this_thread::sleep_for(std::chrono::seconds(2));
    for (int i = 0; i < 10; ++i) 
    {
        std::cout << "Result: " << std::any_cast<int>(futuresVec[i].get()) << std::endl;
    }


    // Reduce the number of threads in the thread pool
    std::cout << "Reducing thread pool size to 2...\n";
    threadPool.ChangeNumWorkinThreads(2);

    for (int i = 0; i < 10; ++i) 
    {
        futuresVec[i] = 
        threadPool.AddTask(TaskWrapper([i]()
                        {
                            std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Simulate work
                            std::cout << "completed by thread: " << std::this_thread::get_id() << "\n";
                            return i * i * i;
                        }));
    }

  
    // Get the results
    std::this_thread::sleep_for(std::chrono::seconds(2));
    for (int i = 0; i < 10; ++i) 
    {
        std::cout << "Result: " << std::any_cast<int>(futuresVec[i].get()) << std::endl;
    }

    return 0;
}