#include <iostream>
#include <fstream>
#include <cassert>

#include "json.hpp"
#include "solver.h"

using json = nlohmann::json;

int main() {
    std::ifstream in("config.json");
    assert(in.good());
    json config = json::parse(in);
    in.close();

    solver s;
    std::valarray<ValType> init = config["init"];
    s.solve(init, config["modeling"]["time"], config["modeling"]["dt"], config["modeling"]["steps"]);
    s.write("output");
    return 0;
}