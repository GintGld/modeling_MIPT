#include <valarray>
#include <string>
#include <cmath>
#include "function.h"
using namespace std;
valarray<ValType> F(ValType t, valarray<ValType> s) {
	return valarray<ValType> {s[1],  (-1000.0*cos(100*t) - 9.81)*sin(s[0]) };
}
