#include <iostream>
#include <random>

template<class T>
bool compare(const T* h1, const T* h2, int n) {
    bool f = 1;
    for (int i = 0; i < n; ++i)
        f &= (h1[i] == h2[i]);
    return f;
}

int main(int argc, char** argv) {
    int k = 1, comb_size = 0, repeat = 1;
    int* target_comb = nullptr;
    unsigned long long int max_number_of_modeling = 1'000'000'000'000;

    {
        bool f_range = 0, f_iterations = 0, f_combination = 0;
        std::string comb = "";
        int idx = 1;
        while (idx < argc) {
            if (compare(argv[idx], "--range", 7) || compare(argv[idx], "-r", 2)) {
                try {
                    k = std::stoi(argv[++idx]);
                    f_range = 1;
                } catch(...) {
                    std::cout << "Error\nUnknown argument for range: " << argv[idx] << std::endl;
                    return -1;
                }
            } else if (compare(argv[idx], "--combination", 13) || compare(argv[idx], "-c", 2)) {
                comb = argv[++idx];
                f_combination = 1;
            } else if (compare(argv[idx], "--max-mod-number", 16) || compare(argv[idx], "-m", 2)) {
                try {
                    max_number_of_modeling = std::stoi(argv[++idx]);
                } catch(...) {
                    std::cout << "Error\nUnknown argument for max-mod-number: " << argv[idx] << std::endl;
                    return -1;
                }
            } else if (compare(argv[idx], "--iterations", 12) || compare(argv[idx], "-i", 2)) {
                try {
                    repeat = std::stoi(argv[++idx]);
                } catch(...) {
                    std::cout << "Error\nUnknow argument for repeat: " << argv[idx] << std::endl;
                    return -1;
                }
            } else {
                std::cout << "Error\nUnknown argument: " << argv[idx] << std::endl;
                return -1;
            }
            ++idx;
        }

        if (!(f_range && f_combination)) {
            std::cout << "Error\nNot enough parameters\n";
            return -1;
        }

        target_comb = new int[comb.size()];
        comb_size = comb.size();
        for (int i = 0; i < comb.size(); ++i)
            if (comb[i] >= '0' && comb[i] <= '9')
                target_comb[i] = comb[i] - '0';
            else {
                std::cout << "Error\nCombination is not a number sequence: " << comb << std::endl;
                delete[] target_comb;
                return -1;
            }
    }

    int* current_comb = new int[comb_size];
    int idx;
    bool next;

    std::default_random_engine gen;
    std::uniform_int_distribution dist(1, k);

    gen.seed(time(NULL));

    for (int times = 0; times < repeat; ++times) {
        next = 0;

        for (int i = comb_size - 1; i >= 0; --i)
            current_comb[i] = dist(gen);

        for (unsigned long long steps = comb_size + 1; steps < max_number_of_modeling; ++steps) {
            if (compare(current_comb, target_comb, comb_size)) {
                std::cout << steps << ' ';
                next = 1;
                break;
            }
            for (idx = comb_size - 1; idx >= 1; --idx)
                current_comb[idx] = current_comb[idx - 1];
            current_comb[0] = dist(gen);
        }
        if (!next)
            std::cout << max_number_of_modeling + 1;
    }

    delete[] current_comb;
    delete[] target_comb;
    return 0;
}