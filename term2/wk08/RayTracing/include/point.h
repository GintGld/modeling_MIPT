#pragma once

#include "MF.h"

struct point {
public:
    MF x, y;

    point(): point(undefined) {}
    point(MF x, MF y): x(x), y(y) {};

    bool operator==(const point& other) const {
        return x==other.x && y==other.y;
    }

    const static point undefined;
};

MF distance(const point&, const point&);