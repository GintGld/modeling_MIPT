#include <iostream>
#include <float.h>

/*
    sum
    1/2**n, n in (1, +inf)
*/

int main()
{
    float d = 0, r;
    bool f = true;
    std::string s = "";
    for (unsigned i = 1; i < 24; ++i)
    {
        r = 1.;
        for (unsigned j = 0; j < i; ++j) r *= 2;
        d = d + 1 / r;
        std::cout.precision(6);
        std::cout << i << " \t" << d << ",\t" <<  1 / r << std::endl;
    }
    //std::cout << "for n = " << n << " result is " <<  d << std::endl;

}