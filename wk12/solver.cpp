#include <iostream>
#include <fstream>
#include <cassert>
#include <valarray>
#include <functional>

#include "solver.h"

solver::solver(): print_level(0) {}

solver::solver(unsigned print_level): print_level(print_level) {}

void solver::solve(std::valarray<ValType> init_state, ValType L, ValType dt, unsigned steps) {
    data.push_back(init_state);
    for(size_t i = 1; i < size_t(L/dt); ++i)
    {
        std::function<std::valarray<ValType> (ValType, std::valarray<ValType>)> f = F;
        data.push_back(step(i * dt, data[data.size()-1], f, dt, METHOD, steps));
        if (print_level >= 2 && (i+1) % (size_t(L/dt) / 10) == 0)
            std::cout << 10*((i+1) / (size_t(L/dt) / 10)) << "\% done\n";
    }
    if (print_level >= 1)
        std::cout << "modeling done\n";
    return;
}

void solver::clear() {data.clear();}

void solver::write(std::string file) {
    std::ofstream out(file+".binary", std::ios::binary);
    assert(out.good());
    for (size_t i = 0; i < data.size(); ++i)
        for (size_t j = 0; j < data[i].size(); ++j)
            out.write((char*)&data[i][j], sizeof(ValType));
        //out.write((char*)&data[i], sizeof(std::valarray<ValType>));
    out.close();
    if(print_level >= 1)
        std::cout << "data was written to " << file << std::endl;
    return;
}