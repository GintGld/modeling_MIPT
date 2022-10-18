#include <iostream>
#include <float.h>
#include <cmath>

/*
    integral
    int e^(-t^2) dt from 0 to x
    the step early become very small
    counted:        0.886319
    teory:          0.886227
*/

float func(float x)
{
    return exp(- x * x);
}

void int_tranc(float x, long long int N)
{
    float d1 = 0, d2 = 0, d = 0, h = x / static_cast<float>(N), st;
    bool f = true;
    std::string s = "\t";

    d1 = func(0);
    for (unsigned i = 1; i <= N && f; ++i)
    {
        d2 = func(i * h);
        st = 0.5 * h * (d1 + d2);
        d += st;
        if (st <= FLT_MIN)
        {
            std::cout << "on " << i << " iteration step became too small\n";
            f = false;
        }
        d1 = d2;
    }
    std::cout << std::scientific << "for n = " << N << s << "x = " << x << s << " result is " <<  d << std::endl;
}

int main()
{
    int_tranc(100, 100);
}