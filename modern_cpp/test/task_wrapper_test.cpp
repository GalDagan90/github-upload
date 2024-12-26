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

int main()
{
    TestCtor();

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

}