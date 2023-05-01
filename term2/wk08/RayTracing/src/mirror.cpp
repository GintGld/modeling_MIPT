#include "mirror.h"

// flat mirror

flat_mirror::flat_mirror(point p1, point p2): line_segment(p1, p2) {}

point flat_mirror::find_intersection(const ray& r) const {
    return line_segment::find_intersection(r);
}

void flat_mirror::interact(ray& r, space& s) const {
    return mirror::interact(r, s);
}

void flat_mirror::reflection(ray& r) const {
    MF dx = p2.x - p1.x, dy = p2.y - p1.y;
    MF proj = (r.k.x * dx + r.k.y * dy) / (dx * dx + dy * dy);

    r.k = point(2 * proj * dx - r.k.x, 2 * proj * dy - r.k.y);

    return;
}

// spherical mirror
/*point sphere_mirror::find_intersection(const ray&) const {return point::undefined;}

void sphere_mirror::reflection(ray& R) const {
    return;
}


sphere_mirror::sphere_mirror(point center, MF radius, MF phi1, MF phi2):
        sphere_segment(center, radius, phi1, phi2) {}*/