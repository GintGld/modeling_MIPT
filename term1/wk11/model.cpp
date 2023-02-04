#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <cassert>
#include <array>
#include "C:\src\json\single_include\nlohmann\json.hpp"

#include "physics.h"

using std::cout;
using std::endl;
using std::size_t;
using json = nlohmann::json;
/*
template<class T>
class generic_euler
{
    public:
    T operator()(T st, double time, double dt) const
    {
        return st + st.ev(time) * dt;
    }
};
template<class T>
class generic_heun
{
    public:
    T operator()(T st, double time, double dt) const
    {
        T tmp = st + st.ev(time) * dt;
        return st + (st.ev(time) + tmp.ev(time + dt)) * (0.5 * dt);
    }
};
template<class T>
class generic_rk
{
    public:
    T operator()(T st, double time, double dt) const
    {
        T k1 = st.ev(time);
        T k2 = (st + k1 * (0.5 * dt)).ev(time + 0.5 * dt);
        T k3 = (st + k2 * (0.5 * dt)).ev(time + 0.5 * dt);
        T k4 = (st + k3 * dt).ev(time + dt);
        return st + (k1 + k2 * 2 + k3 * 2 + k4) * (dt / 6);
    }
};


template<class T>
class solver
{
    private:
    std::vector<T> data;
    const unsigned print_level;
    double time;

    public:

    solver (unsigned print_level = 0): print_level(print_level), time(0) {}
    T& operator[](size_t i)
    {
        assert(i >= 0 && i < data.size());
        return data[i];
    }
    template<class gen>
    void solve(T init_state, double L, double dt)
    {
        gen f;
        data.push_back(init_state);
        for(size_t i = 1; i < size_t(L/dt); ++i)
        {
            data.push_back(f(data[data.size()-1], i * dt, dt));
            //time += dt;
            if (print_level >= 2)
                if ((i+1) % (size_t(L/dt) / 10) == 0)
                    cout << 10*((i+1) / (size_t(L/dt) / 10)) << "\% done\n";
        }
        if (print_level >= 1)
            cout << "modeling done\n";
    }
    void clear() {data.clear();}
    void write(std::string file)
    {
        std::ofstream out(file, std::ios::binary);
        assert(out.good());
        for (size_t i = 0; i < data.size(); ++i)
        {
            out.write((char*)&(data[i].get_phase()[0]), sizeof(double));
            out.write((char*)&(data[i].get_phase()[1]), sizeof(double));
        }
        out.close();
        if(print_level >= 1)
            cout << "data was written to " << file << endl;
    }
};
*/
#include <array>

int main()
{
    std::array<double, 2> a, b;
    a[0] = 1; a[1] = 2; b[0] = 3; b[1] = 4;
    state<double, 2, 2> st1(a, b), st2(b, b);
    auto s = 2. * st1;
    std::cout << s.phase[0] << ' ' << s.phase[1] << ' ' << s.pars[0] << ' ' << s.pars[1] << "\n";
    std::cout << "fine\n";
    return 0;
    //double x0 = M_PI_4, v0 = 0, g = 1, w = 1, L = 10, dx = 0.01;
    /*
    std::ifstream in("config_file.json");
    assert(in.good());
    json config = json::parse(in);
    in.close();

    double 
        x0 = config["initial coordinate"],
        v0 = config["initial velocity"],
        g  = config["attenuation constant"],
        w0 = config["fundamental frequency"],
        A  = config["force amplitude"],
        w  = config["force frequency"],
        L  = config["modeling interspace"],
        dx = config["step of modeling"];
    bool
        write = config["write"];
    std::string
        type = config["model"];
    unsigned
        print_level = config["print level"];


    physical_pendulum v(x0, v0, g, w0, A, w);
    solver<physical_pendulum> s(print_level);
    bool tp = false;
    if (type == "euler")
    {
        s.solve<generic_euler<physical_pendulum>> (v, L, dx);
        tp = true;
    }
    if (type == "heun")
    {
        s.solve<generic_heun<physical_pendulum>> (v, L, dx);
        tp = true;
    }
    if (type == "rk")
    {
        s.solve<generic_rk<physical_pendulum>> (v, L, dx);
        tp = true;
    }
    if (!tp)
    {
        std::cout << "unknown type of solving method\n";
        return 1;
    }
    
    if (write)
        s.write("modeled_data.binary");
    s.clear();
    return EXIT_SUCCESS;
    */
}