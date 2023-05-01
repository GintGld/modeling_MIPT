#include "geometry.h"

// linear segment
line_segment::line_segment(point p1, point p2):
    p1(p1), p2(p2) {}

point line_segment::find_intersection(const ray& r) const {
    // параметризую отрезок как r = (p2 - p1) * t1 + p1, t1 in [0,1]
    // луч как r = k * t2 + p, t2 in [0, inf]
    point intersection_point = point::undefined;

    MF  den1 = r.k.x * (r.p.y - p1.y) - r.k.y * (r.p.x - p1.x),
        den2 = (r.p.x - p1.x) * (p1.y - p2.y) + (r.p.y - p1.y) * (p2.x - p1.x),
        num = r.k.x * (p2.y - p1.y) - r.k.y * (p2.x - p1.x);

    // случай, если луч и отрезок параллельны,
    // т.е., k || p1p2
    if (num == 0)
        return intersection_point;

    MF t1 = den1 / num, t2 = den2 / num;

    // возвращаю точку перечечения, если она есть
    if (0 <= t1 && t1 <= 1 && t2 >= 0)
        intersection_point = point(r.p.x + r.k.x * t2, r.p.y + r.k.y * t2);

    // Проверка на повторение события
    // Костыль на избегание повторений
    if (r.get_last_event() == event(this, intersection_point))
        intersection_point = point::undefined;

    return intersection_point;
}

// spherical segment
sphere_segment::sphere_segment(point center, MF radius, MF phi1, MF phi2):
        center(center), radius(radius), phi1(phi1), phi2(phi2) {}