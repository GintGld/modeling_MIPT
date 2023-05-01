#pragma once

#include "point.h"
#include "ray.h"

class space;

// basic class
class object {
public:
    virtual ~object() {}

    virtual point find_intersection(const ray&) const = 0;
    virtual void interact(ray&, space&) const = 0;
};