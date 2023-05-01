#pragma once
#include <string>
#include <vector>

#include "point.h"

class object;

struct event {
    const object* obj;
    point intersection_point;

    event(): event(undefined) {}

    event(const object* obj, point intersection_point);

    bool operator==(const event& other);

    const static event undefined;
};

class ray {
public:
    point p, k;

private:
    std::vector<event> trajectory;
    event last_event;
    bool exist;

public:
    ray(): exist(true) {}
    ray(point, point);

    void move(MF);
    void set_status(point, point);
    std::vector<event> get_trajectory() const;
    event get_last_event() const;
    void commit(event);
    void del();
    bool is_exist();
};