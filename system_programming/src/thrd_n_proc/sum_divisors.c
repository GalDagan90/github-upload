#include <stdio.h>
#include <omp.h>

int main()
{
    int num = 100000000;
    int i = 1;
    int sum = 0;

    for (; i < num; ++i)
    {
        if (0 == num % i)
        {
            sum += i;
        }
    }
    printf("%d\n", sum);

    return (0);
}