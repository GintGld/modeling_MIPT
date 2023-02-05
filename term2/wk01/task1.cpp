#include <iostream>
#include <random>
#include <functional>

using namespace std;

int main(int argc, char** argv) {

    if (argc < 3) {
        cout << "Not enough values\n";
        return -1;
    }

    int k, n;

    try {
        k = stoi(argv[1]);
        n = stoi(argv[2]);
    } catch(...) {
        cout << "Wrong parameters\n";
        return -1;
    }

    std::default_random_engine gen(0);
    std::uniform_int_distribution dist(1, n);
    
    auto rnd = std::bind(dist, gen);

    for (int i = 0; i < k; ++i)
        cout << rnd() << ' ';

    return 0;
}