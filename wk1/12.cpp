#include<iostream>

int main()
{
    int i;
    for (i = 0; i < 10; i += 1)
    {
        std::cout << &i;
    }
    return 0;
}