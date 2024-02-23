
#include <iostream>
#include <string>
#include <thread>
#include <future>
#include <cassert>

#include "thread_safe_q.hpp"

struct ConcreteTask
{
private:
    int m_id;
public:
    explicit ConcreteTask(int n) : m_id(n) {}
    
    int operator()()
    {
        using namespace std;
        cout << "id: " << m_id << endl;
        return m_id;
    }
};

template<typename T>
struct Producer
{
    Producer(ThreadSafeQueue<T>& q) : m_q{q} {}

    void operator()()
    {
        for (int i = 0; i < 5; i++)
        {
            //each producer pushes 5 tasks to the queue
            ConcreteTask concrete_taks{i};
            T pack_task{std::move(concrete_taks)};
            std::this_thread::sleep_for(std::chrono::milliseconds(25));
            m_q.push(std::move(pack_task));
        }
    }

private:
    ThreadSafeQueue<T>& m_q;
};

template<typename T>
struct Consumer
{
    Consumer(ThreadSafeQueue<T>& q) : m_q{q} {}

    void operator()()
    {
        while (true)
        {
            T pack_task;
            m_q.wait_and_pop(pack_task);
            pack_task();

        }
    }

private:
    ThreadSafeQueue<T>& m_q;
};


int main()
{
    using tasks_type = std::packaged_task<int()>; 
    
    ThreadSafeQueue<tasks_type> globalQ;
    std::vector<std::thread> producer_threads_vec;

    assert(globalQ.empty() == true);

    for (size_t i = 0; i < 3; i++)
    {
        //Create 3 producers
        std::thread thr{Producer<tasks_type>{globalQ}};
        producer_threads_vec.emplace_back(std::move(thr));
    }
    
    //create one consumer
    //Consumer<tasks_type> consumer{globalQ};
    std::thread consumer_thr{Consumer<tasks_type>{globalQ}};

    //join threads
    for (auto& it: producer_threads_vec)
    {
        it.join();
    }
    consumer_thr.join();


    return 0;
}