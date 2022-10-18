#include <iostream>
#include <float.h>
#include <cmath>

/*
    value
    sin(x) using Macloren's series
    on the 17-th iteration step becomes so small
    that floats can't describe such number
    so, it doesn't give correct result if |x| > 2pi
*/

float fact(long long int N)
{
    float d = 1;
    for (long long i = 2; i <= N; ++i)
    {
        d *= static_cast<float>(i);
    }
    return d;
}

void sin_mc(float x, long long int N)
{
    float d = 0, fc;
    bool f = true;
    std::string s = "\t";
    for (unsigned i = 0; i <= N && f; ++i)
    {
        fc = pow(x, 2 * i + 1) / fact(2 * i + 1);
        if (i % 2 == 0) d += fc;
        if (i % 2 == 1) d -= fc;
        if (fc <= FLT_MIN)
        {
            //sstd::cout << "on " << i << " iteration step became too small\n";
            f = false;
        }
    }
    if (N == 1e7) s = "\t";
    std::cout << std::scientific << "for n = " << N << s << "x = " << x << s << " result is " <<  d << std::endl;
}

int main()
{
    for (int i = -9; i <= 9; ++i)
    {
        sin_mc(3.14*static_cast<float>(i), 17);
    }
}