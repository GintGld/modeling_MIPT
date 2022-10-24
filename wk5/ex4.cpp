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
    float d = x, st = x;
    bool f = true;
    for (unsigned i = 1; i <= N && f; ++i)
    {
        st *= -x * x / ((static_cast<float>(i)*2)*(static_cast<float>(i)*2+1));
        d += st;
        //if (st <= FLT_MIN)
        //{
        //    //sstd::cout << "on " << i << " iteration step became too small\n";
        //    f = false;
        //}
        //std::cout << d << std::endl;
    }
    std::cout << d;
    //if (N == 1e7) s = "\t";
    //std::cout << std::scientific << "for n = " << N << s << "x = " << x << s << " result is " <<  d << std::endl;
}

int main(int argc, char* argv[])
{
    int n = 17, g = 1;
    float alpha = 1.;
    if (argc > 1) alpha = std::stof(argv[1]);
    if (argc > 2) n = std::stoi(argv[2]);
    if (argc > 3) g = std::stoi(argv[3]);

    sin_mc(alpha, n);
}