#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <vector>
#include <atomic>
#include <chrono>
#include "threadsafe_queue.hpp"


struct S
{
    int m_i;
    std::string m_str;

    S() = default;
    S(int x, const std::string& str) : m_i{x}, m_str{str} {}
};


// Simple test function to push and pop items from the queue
void TestQueue(ThreadsafeQueue<S>& queue, std::atomic<bool>& done) {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));  // Simulate some delay

    // Push some data
    S s1(1, "Gal");
    queue.Push_Back(s1);
    queue.Emplace_Back(2, "Rachel");
    queue.Emplace_Back(3, "Alon");
    queue.Emplace_Back(4, "Yulia");

    done = true;
}

void ConsumerThread(ThreadsafeQueue<S>& queue)
{
    // Wait until the data is available in the queue
    S result;
    if (queue.WaitForPop(result, std::chrono::milliseconds(100)))
        std::cout << "Consumer popped: " << result.m_i  << ", " << result.m_str << std::endl;
    else
        std::cout << "Consumer timed out" << std::endl;


    auto data = queue.WaitForPop(std::chrono::milliseconds(1000));
    if (data)
        std::cout << "Consumer popped: " << data->m_i  << ", " << data->m_str << std::endl;
    else
        std::cout << "Consumer timed out" << std::endl;
    
}


void TestingMultiThreads()
{
    ThreadsafeQueue<S> queue;
    // Atomic variable to signal when pushing is done
    std::atomic<bool> done{false};

    // Create and run producer thread
    std::thread producer(TestQueue, std::ref(queue), std::ref(done));

    // Create and run consumer thread
    std::thread consumer(ConsumerThread, std::ref(queue));

    // Wait for producer to finish
    producer.join();

    // Allow some time for consumers to process
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Set done to true to stop consumers
    done = true;

    // Join consumer threads
    consumer.join();
}

void TestSingleThread()
{
    ThreadsafeQueue<S> queue;
    // Push some data

    S result;
    std::cout << (queue.TryPop(result) ? "Popped something" : "Queue is empty, nothing to pop") << "\n";

    S s1(1, "Gal");
    queue.Push_Back(s1);
    queue.Emplace_Back(2, "Rachel");
    queue.Emplace_Back(3, "Alon");
    queue.Emplace_Back(4, "Yulia");


    bool isDataAvaliable = queue.WaitPop(result);
    if (isDataAvaliable)
        std::cout << "Consumer popped: " << result.m_i  << ", " << result.m_str << std::endl;
    else
        std::cout << "Consumer timed out" << std::endl;

    auto data = queue.WaitPop();
    if (data)
        std::cout << "Consumer popped: " << data->m_i  << ", " << data->m_str << std::endl;
    else
        std::cout << "Consumer timed out" << std::endl;
}


int main() {
    // Create a thread-safe queue
    
    //TestSingleThread();
    TestingMultiThreads();

    return 0;
}
