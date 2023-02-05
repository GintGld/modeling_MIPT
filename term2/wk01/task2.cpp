#include <iostream>
#include <random>
#include <functional>

using namespace std;

int main(int argc, char** argv) {

    if (argc < 3) {
        cout << "Not enough values\n";
        return -1;
    }

    int k;
    float n;

    try {
        k = stoi(argv[1]);
        n = stod(argv[2]);
    } catch(...) {
        cout << "Wrong parameters\n";
        return -1;
    }

    std::default_random_engine gen(0);
    std::uniform_real_distribution dist((float)(1), n);

    auto rnd = std::bind(dist, gen);

    for (int i = 0; i < k; ++i)
        cout << rnd() << ' ';

    return 0;
}