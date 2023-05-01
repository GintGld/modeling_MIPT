#include "RayTracing/RayTracing.h"

#include <iostream>

using std::cout, std::endl;

int main() {
    space s(10);

    s.add_ray(ray(point(0, 0), point(1, 0)));
    s.add_object(new flat_mirror(point(0, 1), point(1, 0)));
    s.add_object(new flat_mirror(point(0, -1), point(1, 0)));

    s.simulate();

    auto v = s.get_history();

    cout << "Collision history\n";

    point p;
    for (const std::vector<event>& tr : v) {
        for (const event& e : tr) {
            p = e.intersection_point;
            cout << "(" << p.x << ", " << p.y << ") ";
        }
        cout << endl;
    }

    return 0;
}