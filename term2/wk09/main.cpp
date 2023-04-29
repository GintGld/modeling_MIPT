#include <iostream>
#include <fstream>

#include "json.hpp"

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
    double Net [Ny][Nx] = {0};
    double Net_tmp [Ny][Nx];

    // counting
    double lapl_x, lapl_y, left_val, right_val;

    for (int step = 0; step < Number_steps; ++step) {
        // Calculate new values into Net_tmp
        for (int i = 0; i < Ny; ++i) {
            for (int j = 0; j < Nx; ++j) {
                // laplasian x
                if (j == 0)
                    left_val = phi0;
                else
                    left_val = Net[i][j - 1];
                if (j == Nx - 1)
                    right_val = phi1;
                else
                    right_val = Net[i][j + 1];
                lapl_x = (left_val + right_val - 2 * Net[i][j]) / (x_step * x_step);

                // laplasian y
                if (i == 0)
                    left_val = Net[i][j];
                else
                    left_val = Net[i - 1][j];
                if (i == Ny - 1)
                    right_val = Net[i][j];
                else
                    right_val = Net[i + 1][j];
                lapl_y = (left_val + right_val - 2 * Net[i][j]) / (y_step * y_step);
                // count step
                Net_tmp[i][j] = t_step * (lapl_x + lapl_y);
            }
        }

        // Update Net
        for (int i = 0; i < Ny; ++i)
            for (int j = 0; j < Nx; ++j)
                Net[i][j] += Net_tmp[i][j];
    }

    ofstream out(argv[2]);

    if (!out.good()) {
        cout << "Can't open " << file << endl;
        return 1;
    }

    for (int i = 0; i < Ny; ++i) {
        for (int j = 0; j < Nx; ++j) {
            out << Net[i][j];
            if (j < Nx - 1)
                out << ',';
        }
        out << endl;
    }

    out.close();

    return 0;
}