#include <iostream>
#include <random>

using std::cout, std::endl;

int main(int argc, char** argv) {
    int N, k, repeat;

    if (argc < 4) {
        cout << "Not enough pars\n";
        return -1;
    }

    N = std::stoi(argv[1]);
    k = std::stoi(argv[2]);
    repeat = std::stoi(argv[3]);

    std::default_random_engine gen;
    std::uniform_int_distribution dist(0, k - 1);

    gen.seed(time(NULL));

    unsigned long long counter = 0, sub_counter, p;
    bool collecting_set[k];

    for (size_t iter = 0; iter < repeat; ++iter) {
        std::fill(collecting_set, collecting_set + k, false);
        sub_counter = 0;
        for (size_t i = 0; i < N; ++i) if (!collecting_set[p = dist(gen)]) {
                collecting_set[p] = true;
                ++sub_counter;
        }
        //for (int i = 0; i < k; ++i) cout << collecting_set[i] << ' ';
        //cout << endl;
        if (sub_counter == k) ++counter;
    }

    //cout << counter << ' ' << N << ' ' << k << endl;

    cout << static_cast<double>(counter) / static_cast<double>(repeat) / static_cast<double>(N);
}