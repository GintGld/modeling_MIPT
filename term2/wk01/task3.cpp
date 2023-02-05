#include <iostream>
#include <random>
#include <functional>

using namespace std;

int main(int argc, char** argv) {

    if (argc < 4) {
        cout << "Not enough values\n";
        return -1;
    }

    int k;
    double sigma, m;

    try {
        k = stoi(argv[1]);
        m = stod(argv[2]);
        sigma = stod(argv[3]);
    } catch(...) {
        cout << "Wrong parameters\n";
        return -1;
    }

    std::default_random_engine gen(0);
    std::normal_distribution dist{m, sigma};

    auto rnd = std::bind(dist, gen);

    for (int i = 0; i < k; ++i)
        cout << rnd() << ' ';

    return 0;
}