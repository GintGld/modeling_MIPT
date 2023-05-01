#include "physics.h"
#include "space.h"

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