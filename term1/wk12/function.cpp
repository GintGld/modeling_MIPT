#include <valarray>
#include <string>
#include "function.h"
using namespace std;
#undef ValType
#define ValType double
#undef METHOD
#define METHOD (std::string)"rk45"
valarray<ValType> F(ValType t, valarray<ValType> s) {
	return valarray<ValType> {s[2], s[3], 24*(s[2] - s[3])*s[3]*sin(s[0] - s[1]) - 24*s[2]*s[3]*sin(s[0] - s[1]) - 88.29*sin(s[0]) ,  24*(s[2] - s[3])*s[2]*sin(s[0] - s[1]) + 32*s[1] + 24*s[2]*s[3]*sin(s[0] - s[1]) - 78.48*sin(s[1]) };
}
