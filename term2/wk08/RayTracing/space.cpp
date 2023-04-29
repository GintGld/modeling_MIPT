#include "space.h"
#include "ray.h"

#include <limits>
#include <iostream>

unsigned long long space::max_iterations = 10000;

space::space(MF border) {
    p1 = { border / 2,  border / 2};
    p2 = {-border / 2,  border / 2};
    p3 = {-border / 2, -border / 2};
    p4 = { border / 2, -border / 2};
    
    Objects.push_back(new flat_screen {p1, p2});
    Objects.push_back(new flat_screen {p2, p3});
    Objects.push_back(new flat_screen {p3, p4});
    Objects.push_back(new flat_screen {p4, p1});
}

space::~space() {
    delete[] Objects.data();
    for (object* obj: Objects)
        obj = nullptr;
}

void space::delete_ray(ray& r) {
    r.del();
}

void space::update_rays() {
    // Проверка на всякий случай
    if (Rays.size() == 0)
        return;

    // Удаление "мертвого" луча
    for (std::vector<ray>::iterator it = Rays.begin(); it != Rays.end(); ++it) {
        if ( !(*it).is_exist() ) {
            history.push_back( (*it).get_trajectory() );
            Rays.erase(it);
            break;
        }
    }

    // Добавление новых лучей
    Rays.insert(Rays.end(), Rays_to_add.begin(), Rays_to_add.end());
}

void space::make_step(ray& r) {
    MF min_dist, d_tmp;
    point p_inter;
    int id;

    // Поиск ближайшего столкновения
    min_dist = std::numeric_limits<MF>::infinity();
    id = -1;
    for (int i = 0; i < Objects.size(); ++i) {
        p_inter = Objects[i]->find_intersection(r);
        d_tmp = distance(r.p, p_inter);

        // Если событие в этой же точке,
        // то дополнительная проверка
        if (d_tmp == 0) {
            int shift = 1, size = r.get_trajectory().size();
            bool found_cycle = false;

            // Иду вглубь истории, пока события находятся в этой же точке
            while (distance(r.get_trajectory()[size - shift].intersection_point, p_inter) == 0 && !found_cycle) {
                // Если луч уже взаимодействовал с объектом,
                // то событие игнорируется
                found_cycle |= (r.get_trajectory()[size - shift].obj == Objects[i]);

                ++shift;
            }
            if (found_cycle)
                continue;
        }

        if (d_tmp < min_dist) {
            min_dist = d_tmp;
            id = i;
        }
    }

    // Обработка столкновения
    if (id != -1)
        Objects[id]->interact(r, *this);

    return;
}

void space::simulate() {
    unsigned long long counter;

    while (Rays.size() > 0) {
        counter = 0;
        
        make_step(Rays.front());

        if (!Rays.front().is_exist()) {
            update_rays();
        }

        ++counter;

        // Принудительный сброс луча, если
        // он обсчитывается слишком долго
        if (counter == space::max_iterations) {
            Rays.front().del();
            update_rays();
            std::cout << "The ray were living too long.\n" << "It was killed\n";
            continue;
        }
    }
}