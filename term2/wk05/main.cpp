#include "gas1D/gas1D.hpp"
#include <iostream>

using std::cout, std::endl;

int main(int argc, char** argv)
{
    // get config file
    std::string file, prefix = "./";
    if (argc >= 2) {
        file = argv[1];
    } else {
        cout << "Not enough parameters\n" << "Give a path to configuration file.\n";
        return 1;
    }

    // simulate
    gas1D g(file);

    g.simulate();

    // write data
    g.write_coordinates("tmp/coordinates");
    g.write_collisions("tmp/collisions");

    return 0;
}