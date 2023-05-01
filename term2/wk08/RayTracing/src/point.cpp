#include "point.h"

#include <limits> // std::numeric_limits<MF>::infinity
#include <cmath> // sqrt

const point point::undefined(std::numeric_limits<MF>::infinity(), std::numeric_limits<MF>::infinity());

MF distance(const point& p1, const point& p2) {
    return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}