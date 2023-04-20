#pragma once

#include <string>
#include <vector>
#include <initializer_list>

#include "json.hpp"
#include "log.hpp"

/*
    TODO: реализовать вывод классов в консоль
*/

using MF = double;
using json = nlohmann::json;

struct particleType {
public:
    unsigned id;
    MF mass;

    particleType(): id(0), mass(0) {}
    particleType(unsigned id, MF mass):
        id(id), mass(mass) {}
};

struct particle1D: public particleType{
    MF x, xdot;

    particle1D(): particleType(), x(0), xdot() {}
    particle1D(particleType type, MF x = 0, MF xdot = 0): particleType(type), x(x), xdot(xdot) {}
};

struct collision {
    int first, second;
    MF time_step, coord;
    std::string type; // "unknown", "left-reflection", "right-reflection"

    collision(): first(-1), second(-1), time_step(-1), type("unknown") {}; // default constructor
    collision(unsigned id, MF t, MF x, std::string type);
};

class gas1D {
private:
    std::vector<particle1D> Particles;
    unsigned Particle_number;
    std::vector<particleType> Types;

    MF border;

    MF time, current_time, subtime;
    collision nearest_collision;
    std::vector<collision> collision_history;

    json config;
    _message_log log_out;

    int flags;

    std::vector<std::vector<MF> > history_x, history_xdot;
    std::vector<MF> history_x_tmp, history_xdot_tmp, history_time;

public:
    gas1D(): Particles(0) {open_log();}
    gas1D(const std::string&);
    ~ gas1D() {log_out.close();}

private:
    void open_log();
    void init(const std::string&);
    void read_json(const std::string&);
    void browse_main_json_members();
    void generate_types();
    void generate_particles();

public:
    unsigned get_particle_number() {return Particle_number;}
    std::vector<particle1D>& get_particles() {return Particles;}

    // iterable
    std::vector<particle1D>::iterator begin() {return Particles.begin();}
    std::vector<particle1D>::iterator end() {return Particles.end();}
    const particle1D& operator[](int i) {return Particles[i];}

    // modeling
    void make_step();
    void check_collisions();
    void check_reflections();
    void simulate(MF);
    void simulate();

    // saving data
    void save_state();
    void write_coordinates(const std::string& = "coordinates");
    void write_collisions(const std::string& = "collisions");
};

void assert_elements(json&, std::initializer_list<std::string>);

void handle_collision(particle1D&, particle1D&);