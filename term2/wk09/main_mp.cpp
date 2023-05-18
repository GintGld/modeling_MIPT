#include <iostream>
#include <fstream>
#include <chrono>

#include "json.hpp"

#define n_thread 6

// g++ main.cpp -fopenmp -Wall -Wextra -O2 -g -o main_mp

using json=nlohmann::json;
using std::cout, std::endl, std::ifstream, std::ofstream;

int main(int argc, char** argv) {
    // check file name
    if (argc < 2) {
        cout << "Not enough parameters.\n";
        return 1;
    }
    std::string file = "output.csv";
    if (argc >= 3)
        file = argv[2];

    // constants
    int Nx, Ny, Number_steps;

    double phi0, phi1, x_step, y_step, t_step;

    // get info from json
    {
        json config;

        // open stream
        ifstream reader(argv[1]);
        if (!reader.good()) {
            cout << "Can't read file.\n";
            reader.close();
            return 1;
        }

        // parse json file
        try {
            reader >> config;
        } catch(...) {
            cout << "Can't parse json file.\n";
            return 1;
        }

        try {
            Nx = config["Nx"];
            Ny = config["Ny"];
            Number_steps = config["N"];
            phi0 = config["phi0"];
            phi1 = config["phi1"];
            x_step = config["x_step"];
            y_step = config["y_step"];
            t_step = config["t_step"];
        } catch(...) {
            cout << "Error in getting variables.\n";
            return 1;
        }

        reader.close();
    }

    // filling
    double** Net = new double*[Ny + 2];
    double** Net_tmp = new double*[Ny + 2];
    double** sigma = new double*[Ny + 2];

    for (int i = 0; i < Ny + 2; ++i) {
        Net[i] = new double[Nx + 2]{0};
        Net_tmp[i] = new double[Nx + 2];
        sigma[i] = new double[Nx + 2]{1};
    }

    // initiate sigma matrix
    #pragma omp parallel for num_threads(n_thread)
    for (int i = 0; i < Ny + 2; ++i) {
        for (int j = 0; j < Nx + 2; ++j) {
            // Последовательное соединение
            if (2 * j < Nx)
                sigma[i][j] = 1;
            else
                sigma[i][j] = 2;
        }
    }

    // counting
    auto begin = std::chrono::steady_clock::now();
    for (int step = 0; step < Number_steps; ++step) {
        // update buffer
        #pragma omp parallel for num_threads(n_thread)
        for (int i = 1; i <= Ny; ++i) {
            Net[i][0] = phi0;
            Net[i][Nx + 1] = phi1;
        }
        #pragma omp parallel for num_threads(n_thread)
        for (int j = 1; j <= Nx; ++j) {
            Net[0][j] = Net[1][j];
            Net[Ny + 1][j] = Net[Ny][j];
        }

        // Calculate new values into Net_tmp
        #pragma omp parallel for num_threads(n_thread)
        for (int i = 1; i <= Ny; ++i) {
            for (int j = 1; j <= Nx; ++j) {
                // calculate effective sigmas
                double s1 = 2 * sigma[i][j] * sigma[i][j - 1] / (sigma[i][j] + sigma[i][j - 1]);
                double s2 = 2 * sigma[i][j] * sigma[i - 1][j] / (sigma[i][j] + sigma[i - 1][j]);
                double s3 = 2 * sigma[i][j] * sigma[i][j + 1] / (sigma[i][j] + sigma[i][j + 1]);
                double s4 = 2 * sigma[i][j] * sigma[i + 1][j] / (sigma[i][j] + sigma[i + 1][j]);

                // calculate j
                double j1 = (Net[i][j] - Net[i][j - 1]) * s1 / x_step;
                double j2 = (Net[i][j] - Net[i - 1][j]) * s2 / y_step;
                double j3 = (Net[i][j + 1] - Net[i][j]) * s3 / x_step;
                double j4 = (Net[i + 1][j] - Net[i][j]) * s4 / y_step;

                // calculate div(j)
                double lapl_x = (j3 - j1) / x_step;
                double lapl_y = (j4 - j2) / y_step;

                // count step
                Net_tmp[i][j] = t_step * (lapl_x + lapl_y);
            }
        }

        // Update Net
        #pragma omp parallel for num_threads(n_thread)
        for (int i = 1; i <= Ny; ++i)
            for (int j = 1; j <= Nx; ++j)
                Net[i][j] += Net_tmp[i][j];
    }

    auto end = std::chrono::steady_clock::now();

    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);

    std::cout << "The time: " << elapsed_ms.count() << " ms\n";

    ofstream out(argv[2]);

    if (!out.good()) {
        cout << "Can't open " << file << endl;
        return 1;
    }

    for (int i = 1; i <= Ny; ++i) {
        for (int j = 1; j <= Nx; ++j) {
            out << Net[i][j];
            if (j < Nx)
                out << ',';
        }
        out << endl;
    }

    out.close();

    for (int i = 0; i < Ny + 2; ++i) {
        delete[] Net[i];
        delete[] Net_tmp[i];
        delete[] sigma[i];
    }
    delete[] Net;
    delete[] Net_tmp;
    delete[] sigma;

    return 0;
}