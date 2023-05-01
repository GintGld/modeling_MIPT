#include "ray.h"
#include "space.h"

#include <cmath> // sqrt

// event struct
event::event(const object* obj, point intersection_point):
    obj(obj), intersection_point(intersection_point) {}

bool event::operator==(const event& other) {
    return obj == other.obj && intersection_point == other.intersection_point;
}

// ray class
const event event::undefined(nullptr, point::undefined);

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

std::vector<event> ray::get_trajectory() const {
    return trajectory;
}

event ray::get_last_event() const {
    return last_event;
}

void ray::del() {
    exist = false;
}

bool ray::is_exist() {
    return exist;
}