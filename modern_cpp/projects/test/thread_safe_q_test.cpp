#include "thread_safe_q.hpp"

#include <iostream>

class Task
{
    Task();
    void operator()(int x)
    {
        using namespace std;
        cout << "printed: " << x << "\n";
    }
};

int main()
{
    ThreadSafeQueue<Task> q;
    return 0;
}