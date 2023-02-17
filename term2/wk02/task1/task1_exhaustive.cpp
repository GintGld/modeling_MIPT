#include <iostream>

using std::cout, std::endl;

class Searcher {
    int* data;
    bool* finder;
    int size, k;

public:
    Searcher(int N, int k): data(new int[N]), finder(new bool[k]), size(N), k(k) {
        std::fill(data, data+size, 0);
    }

    ~Searcher() {
        delete[] data;
        delete[] finder;
    }

    void step() {
        data[size - 1] += 1;
        simplify();
    }

private:
    void simplify() {
        for (int i = size - 1; i > 0; --i){
            data[i - 1] += data[i] / k;
            data[i] %= k;
        }
        data[0] %= k;
    }

public:
    int find_set() {
        bool f;

        std::fill(finder, finder + k, false);

        for (int i = 0; i < size; ++i) if (!finder[data[i]]) {
            finder[data[i]] = true;

            f = true;

            for (int j = 0; j < k; ++j)
                f &= finder[j];
            if (f)
                return i;
        }
        return -1;
    }

    bool is_final() {
        bool f = true;
        for (int i = 0; i < size; ++i) f &= (data[i] != k);
        return f;
    }
};

int main(int argc, char** argv) {
    unsigned N, k;

    if (argc < 3) {
        cout << "Not enough args\n";
        return -1;
    }

    N = std::stoi(argv[1]);
    k = std::stoi(argv[2]);

    Searcher S(N, k);

    unsigned long long counter = 0, num = 0;
    int idx;

    while(S.is_final()) {
        if (idx = S.find_set() != -1) {
            //counter += std::pow(k, N - idx);
            ++counter;
        }
        ++num;

        S.step();
    }

    cout << counter << ' ' << num << endl;

    cout << static_cast<float>(counter) / static_cast<float>(num);

    return 0;
}