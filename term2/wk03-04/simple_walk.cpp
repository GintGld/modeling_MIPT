#include <iostream>
#include <fstream>
#include <random>

template<class T>
bool compare(const T* h1, const T* h2, int n) {
    bool f = 1;
    for (int i = 0; i < n; ++i)
        f &= (h1[i] == h2[i]);
    return f;
}

int main(int argc, char** argv) {
    int N_iterations = 10, N_tries = 1;
    std::string file_name = "result";

    {
        int idx = 1;
        while (idx < argc) {
            if(compare(argv[idx], "-I", 2) || compare(argv[idx], "-i", 2)) {
                try {
                    N_iterations = std::stoi(argv[++idx]);
                } catch(...) {
                    std::cout << "Error\nUnknown argument for number of iterations: " << argv[idx] << std::endl;
                    return -1;
                }
            }
            if (compare(argv[idx], "-N", 2) || compare(argv[idx], "-n", 2)) {
                try {
                    N_tries = std::stoi(argv[++idx]);
                } catch(...) {
                    std::cout << "Error\nUnknown argument for number of tries: " << argv[idx] << std::endl;
                    return -1;
                }
            }
            if (compare(argv[idx], "-F", 2) || compare(argv[idx], "-f", 2)) {
                file_name = argv[++idx];
            }
            ++idx;
        }
    }

    std::default_random_engine gen;
    std::uniform_int_distribution dist(0, 1); // generates {0, 1}

    gen.seed(time(NULL));

    int current_pos;

    std::ofstream out_str(file_name + ".binary", std::ios::binary);

    for (unsigned long long n = 0; n < N_tries; ++n) {
        current_pos = 0;
        for (unsigned long long i = 0; i < N_iterations; ++i) {
            out_str.write((char*)(&current_pos), sizeof(int));
            current_pos += 2 * dist(gen) - 1;
        }
    }

    out_str.close();
    
    return 0;
}