#include <iostream>
#include <exception> // runtime_error

#include "gas1D.hpp"

// crutch, any other did not work
#define block_delimiter "******************************\n"

collision::collision(unsigned id, MF t, MF x, std::string type):
time_step(t), coord(x), type(type) {
    if (type == "left-reflection") {
        first = -1;
        second = 0;
    } else if (type == "right-reflection") {
        first = id;
        second = id + 1;
    } else if (type == "collision") {
        first = id;
        second = id + 1;
    } else {
        type = "unknown";
        first = -1000;
        second = -1000;
        time_step = -1000;
    }
}

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
    /*
        Parsing file
    */
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
    /*
        Check the needed structure of json file
    */

    try {
        assert_elements(config, {"types", "particles", "border"});
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

    log_out << "\n";
}

void gas1D::generate_types() {
    /*
        Generating particle types
    */

    log_out << "Generating types\n";

    // Check indices for repeating and throw error if it is
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
            assert_elements(config["types"][i], {"mass"});
        } catch (std::runtime_error& e) {throw e;}
        try {
            Types.push_back(
                particleType(
                    config["types"][i]["id"],
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
    /*
        Generating particles
    */

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

void gas1D::simulate(MF m_time) {
    /*
        Simple modeling loop
        with saving coordinates and 
        velocities at every step
    */

    log_out << "Start simulating.\n" << "Time:\t\t\t\t" << m_time << "\n";
    
    if (m_time < 0)
        return;

    current_time = 0;
    time = m_time;

    while(current_time < m_time){
        save_state();
        make_step();
    }
    save_state();
    
    log_out << "\n" << "Finish simulation\n"
            << block_delimiter;
}

void gas1D::simulate() {
    simulate(time);
}

void gas1D::make_step() {
    /*
        Making a step in model,
        searching the neaerest event
        and make step with those time
    */

    // define default values
    subtime = time - current_time;
    nearest_collision = collision(-10, -10, -10, "unknown");

    // searching for nearest event
    check_collisions();
    check_reflections();

    // handle the nearest event or 
    // just move particle if there is no any
    if (nearest_collision.type == "collision") {               // collision
        unsigned id_1 = nearest_collision.first,
                id_2 = nearest_collision.second;
        Particles[id_1].x = nearest_collision.coord;
        Particles[id_2].x = nearest_collision.coord;
        handle_collision(Particles[id_1], Particles[id_2]);
    } else if (nearest_collision.type == "left-reflection") {  // left reflection
            Particles.front().x = 0;
            Particles.front().xdot *= -1;
    } else if (nearest_collision.type == "right-reflection") { // right reflection
            Particles.back().x = border;
            Particles.back().xdot *= -1;
    }

    // move other particles
    for (int i = 0; i < Particle_number; ++i) {
        if (i != nearest_collision.first && i != nearest_collision.second)
            Particles[i].x += subtime * Particles[i].xdot;
    }

    // make step in time
    current_time += subtime;

    // add to history (with absolute time)
    nearest_collision.time_step = current_time;
    collision_history.push_back(nearest_collision);
}

void gas1D::check_collisions() {
    /*
        Going through particles' collisions
    */
    for (unsigned i = 0; i < Particle_number - 1; ++i) {
        MF  x1 = Particles[i].x, x2 = Particles[i + 1].x,
            v1 = Particles[i].xdot, v2 = Particles[i + 1].xdot;

        if (v1 >= 0 && v2 <= 0 && (v1 != 0 || v2 != 0)) { // check dimensions
            if (x2 - x1 < (v1 - v2) * subtime) { // chech if this collision will be earlier than others
                subtime = (x2 - x1) / (v1 - v2); // update time step
                nearest_collision = collision(   // update target collision
                    i, 
                    subtime, 
                    (x2 * v1 - x1 * v2) / (v1 - v2),
                    "collision"
                );
            }
        }
    }
}

void gas1D::check_reflections() {
    /*
        Check left and right reflections
    */

    if (Particles.front().xdot < 0) { // left reflection
        if (Particles.front().x < - Particles.front().xdot * subtime) {
            subtime = - Particles.front().x / Particles.front().xdot;
            nearest_collision = collision(
                0, 
                subtime,
                0,
                "left-reflection"
            );
        }
    }
    if (Particles.back().xdot > 0) { // right reflection
        if (border - Particles.back().x < Particles.back().xdot * subtime) {
            subtime = (border - Particles.back().x) / Particles.back().xdot;
            nearest_collision = collision(
                Particle_number - 1, 
                subtime,
                border,
                "right-reflection"
            );
        }
    }
}

void gas1D::save_state() {
    /*
        Saving state of system at given time
        if coordinates have not changed since
        previous iteration, updates velocities
    */

    for(const particle1D& p : Particles) {
        history_x_tmp.push_back(p.x);
        history_xdot_tmp.push_back(p.xdot);
    }
    if (history_x.size() > 0 && history_x_tmp == history_x.back()) {
        history_xdot.back() = history_xdot_tmp;
    } else {
        history_x.push_back(history_x_tmp);
        history_xdot.push_back(history_xdot_tmp);
        history_time.push_back(current_time);
    }
    history_x_tmp.clear();
    history_xdot_tmp.clear();
}

void gas1D::write_coordinates(const std::string& filename) {
    /*
        writes save data in .csv format
        the structure is:
            `time,x1,x2,...,xn,xdot1,xdot2,...,xdotn`
        where n - number of particles
    */

    std::ofstream out;
    std::string postfix = ".csv";
    
    out.open(filename+postfix);

    if (!out.good()) {
        log_out << "Cannot open stream to " << filename << postfix << " file\n";
        return;
    }

    // header
    out << "time,";
    for (unsigned i = 0; i < Particle_number; ++i)
        out << "x" << i << ",";
    for (unsigned i = 0; i < Particle_number - 1; ++i)
        out << "xdot" << i << ",";
    out << "xdot" << Particle_number - 1 << std::endl;

    for (unsigned i = 0; i < history_time.size(); ++i) {
        out << history_time[i] << ',';
        for (unsigned j = 0; j < history_x[i].size(); ++j)
            out << history_x[i][j] << ',';
        for (unsigned j = 0; j < history_x[i].size() - 1; ++j)
            out << history_xdot[i][j] << ',';
        out << history_xdot[i][Particle_number - 1] << std::endl;
    }

    out.close();

    log_out << "Data written to " << filename << postfix << "\n"
            << block_delimiter;
}

void gas1D::write_collisions(const std::string& filename) {
    /*
        writes save data in .csv format
        the structure is:
            `time,coll`
        where `coll` - number of collisions happened at `time`
    */

    std::ofstream out;
    std::string postfix = ".csv";
    
    if (collision_history.size() == 0) {
        log_out << "No collisions to write\n.";
        return;
    }
    
    out.open(filename+postfix);

    if (!out.good()) {
        log_out << "Cannot open stream to " << filename << postfix << " file\n";
        return;
    }

    out << "time,coll\n";
    int coll_counter;
    for (const MF& t: history_time) {
        coll_counter = 0;
        for (const collision& coll : collision_history)
            coll_counter += (coll.time_step == t && coll.type != "unknown");
        if (coll_counter != 0)
            out << t << ',' << coll_counter << "\n";
    }

    out.close();

    log_out << "Collisions written to " << filename << postfix << "\n"
            << block_delimiter;
}

void handle_collision(particle1D& p1, particle1D& p2) {
    /*
        calculate the kinematics of elastic collision
    */
    MF m1 = p1.mass, m2 = p2.mass, v1 = p1.xdot, v2 = p2.xdot;
    p1.xdot = (v1 * (m1 - m2) + v2 * 2 * m2) / (m1 + m2);
    p2.xdot = (2 * m1 * v1 + (m2 - m1) * v2) / (m1 + m2);
    return;
}

void assert_elements(json& j, std::initializer_list<std::string> names) {
    /*
        simple check if given elements exist in json
    */
    for (const std::string& name: names)
        if (j[name] == nullptr) {
            throw std::runtime_error("Did not find "+name+" in json file");
        }
    return;
}
