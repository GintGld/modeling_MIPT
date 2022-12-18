#include <vector>
#include <string>
#include <valarray>

#include "function.h"
#include "solving_methods.h"

class solver {
private:
    std::vector<std::valarray<ValType>> data;
    const unsigned print_level;

public:
    solver();
    solver(unsigned);
    void solve(std::valarray<ValType>, ValType, ValType, unsigned);
    void clear();
    void write(std::string);
};