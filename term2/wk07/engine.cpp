#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
using std::cout, std::endl;

bool compare(char* first, std::string second) {
    bool f = true;
    for (int i = 0; i < second.size(); ++i)
        f &= (first[i] == second[i]);
    return f;
}

int main(int argc, char** argv) {
    double x, lambda;
    int N;
    
    {
        bool f_x, f_l, f_n;
        f_x = f_l = f_n = false;
        int i = 1;
        while(i < argc) {
            if (compare(argv[i], "-x")) {
                try {
                    x = std::stod(argv[++i]);
                    f_x = true;
                } catch(...) {throw std::runtime_error("Cannot convert string to float for -x");}
            } else if (compare(argv[i], "-l")) {
                try {
                    lambda = std::stod(argv[++i]);
                    f_l = true;
                } catch(...) {throw std::runtime_error("Cannot convert string to float for -l");}
            } else if (compare(argv[i], "-n")) {
                try {
                    N = std::stoi(argv[++i]);
                    f_n = true;
                } catch(...) {throw std::runtime_error("Cannot convert string to int for -n");}
            }
            ++i;
        }
        if (!f_x || !f_l || !f_n) {
            cout << "Not enough paraameters (-x, -l, -n)\n";
            return 1;
        }
        if (x < 0 || x > 1) {
            cout << "X must be in [0, 1]\n";
        }
    }

    std::vector<double> x_hist;

    for (int i = 0; i < N; ++i) {
        x = lambda * x * (1 - x);
        x_hist.push_back(x);
    }

    std::ofstream out("out.dat", std::ios::binary);
    for (double v : x_hist) {
        out.write((char*)(&v), sizeof(double));
    }
    return 0;
}