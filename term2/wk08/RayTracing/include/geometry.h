#pragma once

#include "point.h"
#include "ray.h"
#include "object.h"

// geometry classes

class line_segment: public virtual object {
protected:
    point p1, p2;

public:
    virtual ~line_segment() {}

    line_segment(point p1, point p2);
    point find_intersection(const ray&) const override;
};

class sphere_segment: public virtual object {
protected:
    point center;
    MF radius, phi1, phi2;

    sphere_segment(point center, MF radius, MF phi1, MF phi2);
};