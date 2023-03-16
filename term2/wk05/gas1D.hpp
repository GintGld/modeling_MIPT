#pragma once

#include <vector>
#include <tuple>
#include <initializer_list>
#include <fstream>
#include <random>
#include <algorithm> // shuffle
#include <exception> // runtime_error

#include "json.hpp"
#include "log.hpp"

/*
    реализовать вывод классов в консоль
*/

using MF = double;
using json = nlohmann::json;

bool check_elements(json&, std::initializer_list<std::string>);

struct particleType {
public:
    std::string name;
    unsigned id;
    MF radius, mass;

    //particleType(): id(++id_counter), name("p"+std::to_string(id_counter)), radius(1), mass(1) {}
    particleType(): id(0), name("null Type"), radius(0), mass(0) {}
    particleType(MF radius, MF mass): 
        id(++id_counter), name("p"+std::to_string(id_counter + 1)), radius(radius), mass(mass) {}
    particleType(const std::string& name, MF radius, MF mass):
        id(++id_counter), name(name), radius(radius), mass(mass) {}

private:
    static unsigned id_counter;
};

struct particle1D : public particleType {
    MF x, xdot;

    particle1D(): x(0), xdot() {}
    particle1D(particleType type): particleType(type), x(0), xdot(0) {}
};

class gas1D {
private:
    unsigned id;

    std::vector<particle1D> Particles;
    unsigned Particle_number;
    std::vector<
    std::tuple<particleType, unsigned>
    > Types;

    MF border;

    json config;
    _message_log log_out;

public:
    gas1D(): id(++id_counter), Particles(0) {open_log();}
    gas1D(const std::string&);
    ~ gas1D() {log_out.close();}

private:
    void open_log();
    bool init(const std::string&);
    bool generate_particles();

public:
    unsigned get_id() {return id;}
    unsigned get_particle_number() {return Particle_number;}
    std::vector<particle1D>& get_particles() {return Particles;}

    std::vector<particle1D>::iterator begin() {return Particles.begin();}
    std::vector<particle1D>::iterator end() {return Particles.end();}
    const particle1D& operator[](int i) {return Particles[i];}

private:
    static unsigned id_counter;
};

void gas1D::open_log() {
    std::string id_s = std::to_string(id);
    log_out.open("gas1D_"+id_s, 
    "gas1D "+id_s+" created", 
    "destroying gas1D "+id_s);
}

bool gas1D::init(const std::string& config_path) {
    /*
        Initialization function.
        Reads file `config_path` as json and 
        tries to use it as configuration file
    */

    std::ifstream input_stream(config_path);

    // Check if file exists
    if (!input_stream.good()) {
        input_stream.close();
        log_out << "Cannot find file " << config_path << "\n";
        return false;
    }

    log_out << "Found file " << config_path << ", start reading\n";

    // Check is file readable
    try {
        config = json::parse(input_stream);
    } catch(nlohmann::json_abi_v3_11_2::detail::parse_error& e) {
        input_stream.close();
        log_out << "nlohmann::json cannot read file " << config_path << "\n";
        return false;
    }

    log_out << "Read file\n";

    input_stream.close();

    // Check if "types" and "sizes" exists
    if (!check_elements(config, {"types", "sizes", "border", "max speed"})) {
        log_out << "json gile does not have required lists (\"types\", \"sizes\", \"border\", \"max speed\")\n";
        std::cout << "brr\n";
        return false;
    }

    // Check if "types" len == "sizes" len
    if (config["types"].size() != config["sizes"].size()) {
        log_out << "Length of lists of types and sizes are not equal. They are "
                << config["types"].size() << " and "
                << config["sizes"].size() << "\n";

        return false;
    }

    log_out << "Generating Types\n";

    // Fulfill `Types` vector
    for (int i = 0; i < config["types"].size(); ++i) {
        if (!check_elements(config["types"][i], {"mass", "radius"})) {
            log_out << "\"types\" list does not have necessary elements at "
                    << i << " position. Elements required: \"mass\", \"radius\"\n";
            return false;
        }
        if (check_elements(config["types"][i], {"names"})) {
            Types.push_back({
            particleType(
                config["types"][i]["name"],
                config["types"][i]["radius"],
                config["types"][i]["mass"]
            ),
            config["sizes"][i]
            });
        } else {
            Types.push_back({
            particleType(
                config["types"][i]["radius"],
                config["types"][i]["mass"]
            ),
            config["sizes"][i]
            });
            //log_message("Type "+std::to_string(i)+" has not own name. It was named "+std::get<0>(Types.back()).name+".");
            log_out << "Type " << i + 1 << " has not own name. It was named " << std::get<0>(Types.back()).name << ".\n";
        }
    }

    log_out << "Types generated\n";

    log_out << "Getting border\n";

    border = config["border"];

    log_out << "Border setted\n";

    if (!generate_particles())
        return false;
    
    return true;
}

bool gas1D::generate_particles() {
    log_out << "Generating coordinates\n";

    std::vector<particleType> particle_order;

    Particle_number = 0;
    for (const auto& [type, size] : Types){
        for (int i = 0; i < size; ++i)
            particle_order.push_back(type);
        Particle_number += size;
    }

    {
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(
            particle_order.begin(),
            particle_order.end(),
            g
        );
    }

    MF distance_between_particles = (border - 2 * (particle_order.front().radius + particle_order.back().radius)) / (Particle_number - 1);

    // Check if double radius is less than `distance_between_particles`
    for (const auto& [type, size] : Types) {
        if (2 * type.radius >= distance_between_particles) {
            log_out << "Size of box is too small\n";
            return false;
        }
    }

    for (const particleType& ord : particle_order)
        Particles.push_back(particle1D(ord));

    MF indent = 2 * particle_order.front().radius;

    std::random_device rd;
    std::mt19937 gen(rd());
    MF max_speed = config["max speed"];
    std::uniform_real_distribution<MF> dist(-max_speed, max_speed);

    // defining x and xdot
    for (int i = 0; i < Particle_number; ++i) {
        Particles[i].x = indent + distance_between_particles * i;
        Particles[i].xdot = dist(gen);
    }

    // Changing to center of mass coordinate system
    MF Sum_p = 0, Sum_m = 0;
    for (const particle1D& p : Particles) {
        Sum_p += p.mass * p.xdot;
        Sum_m += p.mass;
    }
    
    for (int i = 0; i < Particle_number; ++i)
        Particles[i].xdot -= Sum_p / Sum_m;

    return true;
}

gas1D::gas1D(const std::string& config_path): id(++id_counter) {
    open_log();
    if (!init(config_path)) {
        log_out << "Fail in initializing gas1D";
        log_out.close();
        //throw "Error in creating gas1D\nRead .log file for more details\n";
        throw std::runtime_error("Error in creating gas1D\nRead .log file for more details\n");
    }
    log_out << "Initialized gas1D\n";
}

bool check_elements(json& j, std::initializer_list<std::string> names) {
    bool f = true;
    for (const std::string& name : names)
        f &= (j[name] != nullptr);
    return f;
}

// initialization of static parameters
unsigned particleType::id_counter = 0;
unsigned gas1D::id_counter = 0;