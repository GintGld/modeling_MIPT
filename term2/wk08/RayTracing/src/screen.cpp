#include "screen.h"

flat_screen::flat_screen(point p1, point p2):
    line_segment(p1, p2) {}

point flat_screen::find_intersection(const ray& r) const {
    return line_segment::find_intersection(r);
}

void flat_screen::interact(ray& r, space& s) const {
    return screen::interact(r, s);
}