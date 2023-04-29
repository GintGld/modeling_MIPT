#include "ray.h"
#include "space.h"

#include <iostream>
#include <limits> // std::numeric_limits<MF>::infinity
#include <cmath> // sqrt

const point point::undefined(std::numeric_limits<MF>::infinity(), std::numeric_limits<MF>::infinity());
const event event::undefined(nullptr, point::undefined);

MF distance(const point& p1, const point& p2) {
    return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

ray::ray(point p, point k): p(p), k(k), exist(true) {
    commit(event(nullptr, p));
}

void ray::move(MF dist) {
    if (!exist)
        return;

    MF k_norm = sqrt(k.x * k.x + k.y * k.y);

    p.x += dist * k.x / k_norm;
    p.y += dist * k.y / k_norm;

    return;
}

void ray::commit(event e) {
    if (!exist)
        return;

    last_event = e;
    trajectory.push_back(e);
}

// Basic geometry
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

// Mirror
void mirror::interact(ray& r, space&) const {
    point intersection_point = find_intersection(r);

    // Check if there is no intersection
    if (intersection_point == point::undefined)
        return;
        
    r.move(distance(intersection_point, r.p));

    r.commit(event(this, intersection_point));

    reflection(r);

    return;
}

void flat_mirror::reflection(ray& r) const {
    MF dx = p2.x - p1.x, dy = p2.y - p1.y;
    MF proj = (r.k.x * dx + r.k.y * dy) / (dx * dx + dy * dy);

    r.k = point(2 * proj * dx - r.k.x, 2 * proj * dy - r.k.y);

    return;
}

point sphere_mirror::find_intersection(const ray&) const {return point::undefined;}

void sphere_mirror::reflection(ray& R) const {
    std::cout << "rrr\n";
}

// Screen
void screen::interact(ray& r, space& s) const {
    point intersection_point = find_intersection(r);

    // Check if there is no intersection
    if (intersection_point == point::undefined)
        return;

    r.move(distance(intersection_point, r.p));

    r.commit(event(this, intersection_point));

    s.delete_ray(r);

    return;
}