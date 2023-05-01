#include "ray.h"
#include "object.h"

#include <vector>

class space {
private:
    std::vector<object*> Objects;
    std::vector<ray> Rays, Rays_to_add;
    std::vector<ray*> Rays_to_delete;

    std::vector<std::vector<event>> history;

    // points of square, defining border
    point p1, p2, p3, p4;

public:
    space(MF);
    ~space();

    std::vector<std::vector<event>> get_history() {return history;}

    void add_object(object* ob) {Objects.push_back(ob);}
    void add_ray(ray r) {Rays.push_back(r);}

    void delete_ray(ray&);
    // ToDo добавление нового луча
    void update_rays();

    void make_step(ray&);
    void simulate();

    // Предел на количество шагов моделирования для одного луча
    static unsigned long long max_iterations;
};