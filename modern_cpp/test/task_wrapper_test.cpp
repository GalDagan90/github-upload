#include "task_wrapper.hpp"

int Foo(int i, int j);
void Bar(char c);

int main()
{
    TaskWrapper t1(Foo, 5, 1);
    TaskWrapper t2(Bar, 'g');
    TaskWrapper t3([](int n){ return n > 0; }, -2);

    return 0;
}