#include "ray.h"

#include <vector>

// ToDo: добавить проверку на зацикливание
// например, поставить (изменяемое) ограничение
// на количество итераций для одного луча

class space {
private:
    std::vector<object*> Objects;
    std::vector<ray> Rays, Rays_to_add;
    std::vector<ray*> Rays_to_delete; // indeces

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
    // + добавление нового луча
    void update_rays();

    void make_step(ray&);
    void simulate();

    // Предел на количество шагов моделирования для одного луча
    static unsigned long long max_iterations;
};