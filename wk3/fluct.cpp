#include <iostream>
#include <fstream>
#include <vector>

float w = 2;

std::vector <float> f(std::vector <float> y, float x)
{
    return std::vector <float> {y[1], -w*w*x};
}

/*
    x''+w^2x=0
    y = {x, v}
    y_i+1 = y_i + dx*f(y_i, x_i)
*/

int main(int argc, char *argv[])
{
    std::vector <float> y0 = {6400000, 0};
    unsigned N = 100000;
    float dx = 0.0005;
    if (argc > 1) w = std::stof(argv[1]);
    if (argc > 1) y0[0] = std::stof(argv[2]);
    if (argc > 2) y0[1] = std::stof(argv[3]);
    if (argc > 3) N = std::stoi(argv[4]);
    if (argc > 4) dx = std::stoi(argv[5]);
    std::string file = "fluct_" + 
        std::to_string(y0[0]) + '_' + 
        std::to_string(y0[1]) + '_' + 
        std::to_string(N) + '_' + 
        std::to_string(dx) + ".txt";

    std::ofstream fout(file); fout << "x,y\n";

    std::vector < std::vector <float> > data;
    data.push_back(y0);

    std::vector <float> v = {0., 0.};
    for (unsigned i = 0; i < N; ++i)
    {
        v = f(data[data.size()-1], data[data.size()-1][0]);
        /*std::cout << 
            data[data.size()-1][0] << ' ' << data[data.size()-1][1] << ' ' << 
            dx * v[0] << ' ' << dx * v[1] << ' ' << 
            data[data.size()-1][0] + dx * v[0] << ' ' << data[data.size()-1][1] + dx * v[1] << ' ' << "\n";*/
        //v = data[data.size()-1] + dx * f(data[data.size()-1], data[data.size()-1][0]);
        data.push_back(std::vector <float> {data[data.size()-1][0] + dx * v[0], data[data.size()-1][1] + dx * v[1]});
        //std:: cout << data[data.size()-1][0] << ',' << data[data.size()-1][1] << "\n";
    }
    std::cout << "modeling done.\n";

    for (unsigned i = 0; i < data.size(); ++i) fout << data[i][0] << ',' << data[i][1] << "\n";

    fout.close();
    return 0;
}