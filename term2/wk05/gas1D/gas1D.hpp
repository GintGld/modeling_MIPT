#pragma once

#include <vector>
#include <tuple>
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
    MF radius, mass;

    particleType(): id(0), radius(0), mass(0) {}
    particleType(unsigned id, MF radius, MF mass):
        id(id), radius(radius), mass(mass) {}
};

struct particle1D: public particleType{
    MF x, xdot;

    particle1D(): particleType(), x(0), xdot() {}
    particle1D(particleType type, MF x = 0, MF xdot = 0): particleType(type), x(x), xdot(xdot) {}
};

class gas1D {
private:
    std::vector<particle1D> Particles;
    unsigned Particle_number;
    std::vector<particleType> Types;

    MF border;

    MF time_step, time;

    json config;
    _message_log log_out;

    int flags;

    std::vector<std::vector<MF> > history_x, history_xdot;
    std::vector<MF> history_x_tmp, history_xdot_tmp;

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

    std::vector<particle1D>::iterator begin() {return Particles.begin();}
    std::vector<particle1D>::iterator end() {return Particles.end();}
    const particle1D& operator[](int i) {return Particles[i];}

    void make_step(int = 1);
    void check_collisions(int = 1);
    void reflection(int = 1);
    void simulate(MF);
    void simulate();

    // Use more accurate counting may be slower
    void make_step_exact(int = 1);
    void simulate_exact(MF);
    void simulate_exact();

    void write(const std::string& = "output", bool = true);
};

void assert_elements(json&, std::initializer_list<std::string>);

void collision(particle1D& , particle1D&);