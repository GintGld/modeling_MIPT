#pragma once

#include "ray.h"
#include "object.h"
#include "geometry.h"
#include "physics.h"

// Screen classes

class space;

class flat_screen: public screen, line_segment {
public:
    ~flat_screen() {}

    flat_screen(point p1, point p2);

    point find_intersection(const ray& r) const override;
    void interact(ray& r, space& s) const override;
};