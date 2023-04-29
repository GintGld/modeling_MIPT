#pragma once
#include <string>
#include <vector>

using MF=double;

struct point;
struct event;
class ray;
class object;
class space;

struct point {
public:
    MF x, y;

    point(): point(undefined) {}
    point(MF x, MF y): x(x), y(y) {};

    bool operator==(const point& other) const {
        return x==other.x && y==other.y;
    }

    const static point undefined;
};

MF distance(const point&, const point&);

struct event {
    const object* obj;
    point intersection_point;

    event(): event(undefined) {}

    event(const object* obj, point intersection_point):
        obj(obj), intersection_point(intersection_point) {}

    bool operator==(const event& other) {
        return obj == other.obj && intersection_point == other.intersection_point;
    }

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
    std::vector<event> get_trajectory() const {return trajectory;}
    event get_last_event() const {return last_event;}
    void commit(event);
    void del() {exist = false;}
    bool is_exist() {return exist;}
};

// basic classes
class object {
public:
    virtual ~object() {}

    virtual point find_intersection(const ray&) const = 0;
    virtual void interact(ray&, space&) const = 0;
};

// geometry classes
class line_segment: public virtual object {
protected:
    point p1, p2;

public:
    virtual ~line_segment() {}

    line_segment(point p1, point p2): p1(p1), p2(p2) {};
    point find_intersection(const ray&) const override;
};

class sphere_segment: public virtual object {
protected:
    point center;
    MF radius, phi1, phi2;

    sphere_segment(point center, MF radius, MF phi1, MF phi2):
        center(center), radius(radius), phi1(phi1), phi2(phi2) {}
};

// physic classes
class mirror: public virtual object {
public:
    virtual ~mirror() {}

    void interact(ray& r, space&) const override;
    virtual void reflection(ray&) const = 0;
};

class screen: public virtual object {
public:
    virtual ~screen() {}

    // generic realisation
    void interact(ray& r, space&) const override;
};

// API classes
class flat_mirror: public mirror, line_segment {
public:
    ~flat_mirror() {}

    flat_mirror(point p1, point p2): line_segment(p1, p2) {}

    point find_intersection(const ray& r) const override {
        return line_segment::find_intersection(r);
    }
    void interact(ray& r, space& s) {
        return mirror::interact(r, s);
    }
    void reflection(ray&) const override;
};

class sphere_mirror: protected mirror, sphere_segment {
public:
    sphere_mirror(point center, MF radius, MF phi1, MF phi2):
        sphere_segment(center, radius, phi1, phi2) {}
    
    point find_intersection(const ray&) const override;
    void reflection(ray&) const override;
};

class flat_screen: public screen, line_segment {
public:
    ~flat_screen() {}

    flat_screen(point p1, point p2): line_segment(p1, p2) {}

    point find_intersection(const ray& r) const override {
        return line_segment::find_intersection(r);
    }
    void interact(ray& r, space& s) const override {
        return screen::interact(r, s);
    }
};