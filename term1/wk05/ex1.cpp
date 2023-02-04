#include <iostream>
#include <float.h>

/*
    sum
    1/2**n, n in (1, +inf)
*/

int main(int argc, char* argv[])
{
    int n = 24, g = 1;
    if (argc > 1) n = std::stoi(argv[1]);
    if (argc > 2) g = std::stoi(argv[2]);

    float d = 0, r;

    if (g == 1)
    {
        for (unsigned i = 1; i < n; ++i)
        {
            r = 1.;
            for (unsigned j = 0; j < i; ++j) r *= 2;
            d = d + 1 / r;
            //std::cout.precision(6);
            //std::cout << i << " \t" << d << ",\t" <<  1 / r << std::endl;
        }
        std::cout.scientific;
        std::cout << d;
        return 0;
    }
    if (g == -1)
    {
        for (unsigned i = n - 1; i >= 1; --i)
        {
            r = 1.;
            for (unsigned j = 0; j < i; ++j) r *= 2;
            d = d + 1 / r;
            //std::cout.precision(6);
            //std::cout << i << " \t" << d << ",\t" <<  1 / r << std::endl;
        }
        std::cout.scientific;
        std::cout << d;
        return 0;
    }
    std::cout << "wrong parameter\n";
    return 0;

}