#include <iostream>
#include <fstream>
#include <vector>

double w = 2, dx = 0.001;

std::vector <double> f(std::vector <double> y, double x)
{
    return std::vector <double> {y[1], -w*w*x};
}

std::vector <double> Euler(std::vector <double> y)
{
    std::vector <double> v1 = f(y, y[0]);
    return std::vector <double> {y[0] + dx * v1[0], y[1] + dx * v1[1]};
}

std::vector <double> Heun(std::vector <double> y)
{
    std::vector <double> v1 = {0., 0.}, v2 = {0., 0.};
    v1 = f(y, y[0]); // f(y_i, x_i)
    v2[0] = y[0] + dx * v1[0]; // y~ [0]
    v2[1] = y[1] + dx * v1[1]; // y~ [1]
    v2 = f(v2, v2[0]); // f(y~_i+1, x_i+1)

    return std::vector <double> {
        y[0] + 0.5 * dx * (v1[0] + v2[0]), 
        y[1] + 0.5 * dx * (v1[1] + v2[1])};
}

/*
    x''+w^2x=0
    y = {x, v}
    y_i+1 = y_i + dx*f(y_i, x_i)
*/

int main(int argc, char *argv[])
{
    std::vector <double> y0 = {6400000, 0};
    int Len = 10000;
    if (argc > 1) w = std::stod(argv[1]);
    if (argc > 2) y0[0] = std::stod(argv[2]);
    if (argc > 3) y0[1] = std::stod(argv[3]);
    if (argc > 4) Len = std::stoi(argv[4]);
    if (argc > 5) dx = std::stod(argv[5]);
    int N = static_cast<int>(static_cast<double>(Len)/dx);

    std::string file = "C:/Users/coolg/modeling/wk3/data2/fluct_" + 
        std::to_string(static_cast<int>(y0[0])) + '_' + 
        std::to_string(static_cast<int>(y0[1])) + '_' + 
        std::to_string(Len) + '_' + 
        std::to_string(dx).substr(0, std::min(std::to_string(dx).find_last_not_of('0'), std::to_string(dx).find_last_not_of('.'))+1) + ".txt";

    std::vector < std::vector <double> > data1, data2;
    data1.push_back(y0); data2.push_back(y0);

    for (unsigned i = 0; i < N; ++i)
    {
        data1.push_back(Euler(data1[data1.size()-1]));
        data2.push_back(Heun(data2[data2.size()-1]));
    }
    std::cout << "modeling done.\n";

    std::ofstream fout(file); fout << "x_e,v_e,x_h,v_h,w,L,dx\n";
    for (unsigned i = 0; i < data1.size(); ++i) fout << 
        data1[i][0] << ',' << data1[i][1] << ',' << 
        data2[i][0] << ',' << data2[i][1] << ',' <<
        w << ',' << Len << ',' << dx << "\n";
    fout.close();
    std::cout << "data saved.\n";

    return 0;
}