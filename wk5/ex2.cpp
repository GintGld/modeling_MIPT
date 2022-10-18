#include <iostream>
#include <float.h>

/*
    sum
    1/n, n in (1, +inf)
    it converges to 15.4037 in the 10^7 steps
*/

int main()
{
    float d = 0;
    unsigned n = 10;
    bool f = true;
    std::string s = "\t\t";
    for (n = 10; n <= 1e8 && f; n *= 10)
    {
        d = 0.;
        for (unsigned i = 1; i <= n && f; ++i)
        {
            d = d + 1. / static_cast<float>(i);
            //std::cout << 1. / static_cast<double>(i) << std::endl;
            if (1. / static_cast<float>(i) <= FLT_MIN)
            {
                std::cout << "on " << i << " iteration step became too small\n";
                f = false;
            }
        }
        if (n == 1e7) s = "\t";
        std::cout << "for n = " << n << s << " result is " <<  d << std::endl;
    }

}