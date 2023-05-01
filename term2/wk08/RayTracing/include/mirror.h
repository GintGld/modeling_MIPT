#pragma once

#include "ray.h"
#include "object.h"
#include "geometry.h"
#include "physics.h"

// Mirror classes

class space;

class flat_mirror: public mirror, line_segment {
public:
    ~flat_mirror() {}

    flat_mirror(point p1, point p2);

    point find_intersection(const ray& r) const override;
    void interact(ray& r, space& s) const override;
    void reflection(ray&) const override;
};

/*class sphere_mirror: public mirror, sphere_segment {
public:
    ~sphere_mirror() {}

    sphere_mirror(point center, MF radius, MF phi1, MF phi2);
    
    point find_intersection(const ray&) const override;
    void interact(ray& r, space& s) const override;
    void reflection(ray&) const override;
};*/