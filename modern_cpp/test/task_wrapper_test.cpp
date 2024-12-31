#include "task_wrapper.hpp"
#include <iostream>
#include <string>
#include <string_view>

int Sum(int i, int j){
    return i + j;
}

void Print(std::string_view s){
    std::cout << s << std::endl;
}

void TestCtor();
void TestAssign();
void TestInvoke();

int main()
{
    TestCtor();
    //TestAssign();
    //TestInvoke();

    return 0;
}

void TestCtor()
{
    TaskWrapper tw;
    std::cout << "Default Ctor Success\n";

    TaskWrapper t1(Sum, 5, 1);
    TaskWrapper t2(Print, "Gal");
    std::cout << "Ctor Success function pointer\n";
    
    TaskWrapper t3([](int n){ return n > 0; }, -2);
    std::cout << "Ctor Success lambda\n";

    struct S
    {
        float operator()(float r){
            return r * r * 3.14;
        }
    };

    S s;
    TaskWrapper t4(s, 2.5);
    std::cout << "Ctor Success Functor\n";
    TaskWrapper t5(t1);
    std::cout << "Copy Ctor Success\n";
    t5 = t2;
    std::cout << "Copy Assignment Success\n";

    TaskWrapper t6(std::move(t1));
    std::cout << "Move Ctor Success\n";
    t6 = std::move(t2);
    std::cout << "Move Assignment Success\n";
}

void TestAssign()
{
    TaskWrapper tw;
    tw.AssignTask(Sum, 5, 15);
    auto res = tw.Invoke();

    if (res.has_value()){
        std::cout << std::any_cast<int>(res) << std::endl;
    }

    tw.AssignTask(Print, "Gal");
    tw.Invoke();
}


void TestInvoke()
{
    TaskWrapper t1(Sum, 5, 1);
    auto res = t1.Invoke();

    if (res.has_value()){
        std::cout << "....\n";
        std::cout << std::any_cast<int>(res) << std::endl;
    }
}