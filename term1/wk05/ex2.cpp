#include <iostream>
#include <float.h>

/*
    sum
    1/n, n in (1, +inf)
    it converges to 15.4037 in the 10^7 steps
*/

int main(int argc, char* argv[])
{
    int n = 24, g = 1;
    if (argc > 1) n = std::stoi(argv[1]);
    if (argc > 2) g = std::stoi(argv[2]);
    
    float d = 0;
    bool f = true;

    d = 0.;
    if (g == 1)
    {
        for (unsigned i = 1; i <= n; ++i)
        {
            d = d + 1. / static_cast<float>(i);
        }
        std::cout << d;
        return 0;
    }
    if (g == -1)
    {
        for (unsigned i = n; i >= 1; --i)
        {
            d = d + 1. / static_cast<float>(i);
        }
        std::cout << d;
        return 0;
    }
    std::cout << "wrong parameter\n";
    return 0;
}