#include <iostream>
#include <exception> // runtime_error

#include "gas1D.hpp"

// crutch, any other did not work
#define block_delimiter "******************************\n";

gas1D::gas1D(const std::string& config_path) {
    open_log();
    
    try {
        init(config_path);
    } catch(std::runtime_error& e) {
        log_out << "Fail in initializing gas1D\n" << "Stop program\n\n";
        log_out.close();
        throw e;
    }
}

void gas1D::open_log() {
    if (!log_out.open("gas1D", 
    "Start gas1D log\n", 
    "Stop gas1D log")) {
        std::cerr << "Warning: cannot open log writer.\n" << "Nothing will be written to log.\n";
    }
}

void gas1D::init(const std::string& config_path) {
    /*
        Initialization function.
        Reads file `config_path` as json and 
        tries to use it as configuration file
    */

    try {
        read_json(config_path);
    } catch (std::runtime_error& e) {throw e;}

    try {
        browse_main_json_members();
    } catch (std::runtime_error& e) {throw e;}

    try {
        generate_types();
    } catch (std::runtime_error& e) {throw e;}
    
    try {
        generate_particles();
    } catch (std::runtime_error& e) {throw e;}

    log_out << "Initialized gas1D\n"
            << block_delimiter;
}

void gas1D::read_json(const std::string& config_path) {
    std::ifstream input_stream(config_path);

    // Check if file exists and connection is good
    if (!input_stream.good()) {
        input_stream.close();
        log_out << "Cannot find file " << config_path << "\n";
        throw std::runtime_error("Cannot find file "+config_path);
    }

    log_out << "Found file " << config_path << "\n" << "Start reading\n";

    // Check if file is readable and there is no errors in json syntax
    try {
        config = json::parse(input_stream);
    } catch(nlohmann::json_abi_v3_11_2::detail::parse_error& e) {
        input_stream.close();
        log_out << "nlohmann::json cannot read file " << config_path << "\n";
        throw std::runtime_error("nlohmann::json cannot read file "+config_path);
    }

    log_out << "File parsed\n\n";

    input_stream.close();
}

void gas1D::browse_main_json_members() {
    try {
        assert_elements(config, {"types", "particles", "border", "time step"});
    } catch(std::runtime_error& e) {throw e;}

    try {
        assert_elements(config, {"time"});
        time = config["time"];
        log_out << "Found default simulation time value\n";
    } catch(std::runtime_error& e) {
        log_out << "Did not found default simulation value\n";
    }

    border = config["border"];

    log_out << "Border setted\n";

    time_step = config["time step"];

    log_out << "Time step setted\n";

    log_out << "\n";
}

void gas1D::generate_types() {
    log_out << "Generating types\n";

    // Check indices for repeating and throw error if yes
    std::vector<unsigned> indices_check;
    for (long long unsigned i = 0; i < config["types"].size(); ++i) {
        try {
            assert_elements(config["types"][i], {"id"});
        } catch (std::runtime_error& e) {throw e;}
        for (const unsigned k : indices_check)
            if (k == config["types"][i]["id"])
                throw std::runtime_error("Repeating indices in json configuration file");
        indices_check.push_back(config["types"][i]["id"]);
    }
    indices_check.clear();

    log_out << "Ids are correct\n";

    // Fulfill `Types` vector
    for (long long unsigned i = 0; i < config["types"].size(); ++i) {
        try {
            assert_elements(config["types"][i], {"mass", "radius"});
        } catch (std::runtime_error& e) {throw e;}
        try {
            Types.push_back(
                particleType(
                    config["types"][i]["id"],
                    config["types"][i]["radius"],
                    config["types"][i]["mass"]
                )
            );
        } catch (...) {
            throw std::runtime_error("Invalid values for crerate particleType with id "
                +std::to_string((long long unsigned)(config["types"][i]["id"])));
        }
    }

    log_out << "Types generated\n\n";
}

void gas1D::generate_particles() {
    log_out << "Generating particles\n";

    Particle_number = config["particles"].size();

    log_out << "Found " << Particle_number << " particles\n";

    bool found;

    for (unsigned i = 0; i < Particle_number; ++i) {
        try {
            assert_elements(config["particles"][i], {"x", "xdot", "type id"});
        } catch(std::runtime_error& e) {throw e;}
        
        found = false;

        for (const particleType& type : Types) {
            if (type.id == config["particles"][i]["type id"]) {
                Particles.push_back(
                    particle1D(
                        type,
                        config["particles"][i]["x"],
                        config["particles"][i]["xdot"]
                    )
                );
                found = true;
                break;
            }
        }
        if (!found) {
            log_out << "No match for type id: " << config["particles"][i]["type id"] << "\n";
            throw std::runtime_error("No match for type id: "+std::to_string((unsigned long long)(config["particles"][i]["type id"])));
        }
    }

    log_out << "Particles generated\n\n";
}

void gas1D::simulate(MF time) {
    log_out << "Start simulating.\n" << "Time:\t\t\t\t" << time << "\n"
            << "Number of steps:\t" << int(time / time_step) << "\n"
            << "Time step:\t\t\t" << time_step << "\n\n";
    
    for (unsigned long long i = 0; i * time_step < time; ++i)
        make_step(i + 1);

    log_out << "\n" << "Finish simulation\n"
            << block_delimiter;
}

void gas1D::make_step(int step_num) {
    check_collisions(step_num);
    reflection(step_num);

    for (particle1D& p : Particles) {
        history_x_tmp.push_back(p.x);
        history_xdot_tmp.push_back(p.xdot);
        p.x += p.xdot * time_step;
    }
    history_x.push_back(history_x_tmp);
    history_xdot.push_back(history_xdot_tmp);
    history_x_tmp.clear();
    history_xdot_tmp.clear();
}

void gas1D::check_collisions(int step_num) {
    for (unsigned i = 0; i < Particle_number; ++i) {
        for (unsigned j = i + 1; j < Particle_number; ++j) {
            if (abs(Particles[i].x - Particles[j].x) < Particles[i].radius + Particles[j].radius) {
                collision(Particles[i], Particles[j]);
                log_out << "Collision: " << i + 1 << ", " << j + 1 << " at " << step_num << " step\n";
            }
        }
    }
}

void gas1D::reflection(int step_num) {
    for (unsigned i = 0; i < Particle_number; ++i) {
        if (Particles[i].x < 0 && Particles[i].xdot < 0) {
            Particles[i].xdot *= -1;
            log_out << "Left reflection: " << i + 1 << " at " << step_num << " step\n";
        }
        if (Particles[i].x > border && Particles[i].xdot > 0) {
            Particles[i].xdot *= -1;
            log_out << "Right reflection: " << i + 1 << " at " << step_num << " step\n";
        }
    }
}

void gas1D::write(const std::string& filename, bool binary) {
    std::ofstream out;
    std::string postfix;
    if (binary) {
        postfix = ".data";
        out.open(filename+postfix, std::ios::binary);
    }
    else {
        postfix = ".csv";
        out.open(filename+postfix);
    }

    if (!out.good()) {
        log_out << "Cannot open stream to " << filename << postfix << " file\n";
        return;
    }

    if (binary) {
        for (unsigned long long i = 0; i < history_x.size(); ++i) {
            //for (unsigned j = 0; j < Particle_number; ++j)
            out.write((char*)(&history_x), sizeof(MF) * Particle_number);
            out.write((char*)(&history_xdot), sizeof(MF) * Particle_number);
        }
    }
    else {
        // header
        for (unsigned j = 0; j < Particle_number; ++j)
            out << "x" << j + 1 << ",";
        for (unsigned j = 0; j < Particle_number - 1; ++j)
            out << "xdot" << j + 1 << ",";
        out << "xdot" << Particle_number << "\n";
        

        // data
        for (unsigned long long i = 0; i < history_x.size(); ++i) {
            for (unsigned j = 0; j < Particle_number; ++j)
                out << history_x[i][j] << ",";
            for (unsigned j = 0; j < Particle_number - 1; ++j)
                out << history_xdot[i][j] << ",";
            out << history_xdot[i].back() << "\n";
        }
    }

    out.close();

    log_out << "Data written to " << filename << postfix << "\n"
            << block_delimiter;
}

void collision(particle1D& p1, particle1D& p2) {
    MF m1 = p1.mass, m2 = p2.mass, v1 = p1.xdot, v2 = p2.xdot;
    p1.xdot = (v1 * (m1 - m2) + v2 * 2 * m2) / (m1 + m2);
    p2.xdot = (2 * m1 * v1 + (m2 - m1) * v2) / (m1 + m2);
    return;
}

void assert_elements(json& j, std::initializer_list<std::string> names) {
    for (const std::string& name: names)
        if (j[name] == nullptr) {
            throw std::runtime_error("Did not find "+name+" in json file");
        }
    return;
}
