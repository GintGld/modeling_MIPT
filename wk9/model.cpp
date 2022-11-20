#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <cassert>
#include <array>
#include "C:\src\json\single_include\nlohmann\json.hpp"

using std::cout;
using std::endl;
using std::size_t;
using json = nlohmann::json;

class physical_pendulum
{
    /*
        x'' + 2gx' + w^2 sin(x) = 0

        phase = {x, x'}
        pars  = {g, w}
    */
private:
    std::array<double, 2> phase;
    std::array<double, 2> pars;
    size_t phase_size = 2, pars_size = 2;
public:
    physical_pendulum()
    {
        for (size_t i = 0; i < phase_size; ++i)
            phase[i] = 0;
        for (size_t i = 0; i < pars_size; ++i)
            pars[i] = 0;
    }
    physical_pendulum(double x1, double x2, double x3, double x4)
    {
        phase[0]=x1; phase[1]=x2; pars[0]=x3; pars[1]=x4;
    }

    void operator=(physical_pendulum other)
    {
        phase = other.phase; pars = other.pars;
    }
    physical_pendulum operator+(physical_pendulum other)
    {
        assert(pars == other.pars);
        return physical_pendulum(phase[0]+other.phase[0], phase[1]+other.phase[1], pars[0], pars[1]);
    }
    physical_pendulum operator*(double d)
    {
        return physical_pendulum(phase[0]*d, phase[1]*d, pars[0], pars[1]);
    }
    physical_pendulum ev(double dt)
    {
        return physical_pendulum(phase[1], -2*pars[0]*phase[1]-pars[1]*pars[1]*sin(phase[0]), pars[0], pars[1]);
    }
    std::array<double,2> get_phase()
    {
        return phase;
    }
};

template<class T>
class generic_euler
{
    public:
    T operator()(T st, double dt) const
    {
        return st + st.ev(dt)*dt;
    }
};
template<class T>
class generic_heun
{
    public:
    T operator()(T st, double dt) const
    {
        T tmp = st + st.ev(dt) * dt;
        return st + (st.ev(dt) + tmp.ev(dt))*(0.5*dt);
    }
};
template<class T>
class generic_rk
{
    public:
    T operator()(T st, double dt) const
    {
        T k1 = st.ev(dt);
        T k2 = (st + k1*(0.5*dt)).ev(0.5*dt);
        T k3 = (st + k2*(0.5*dt)).ev(0.5*dt);
        T k4 = (st + k3*dt).ev(dt);
        return st + (k1+k2*2+k3*2+k4)*(dt/6);
    }
};


template<class T>
class solver
{
    private:
    std::vector<T> data;
    const unsigned print_level;
    public:

    solver (unsigned print_level = 0): print_level(print_level) {}
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
            data.push_back(f(data[data.size()-1], dt));
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

int main()
{
    //double x0 = M_PI_4, v0 = 0, g = 1, w = 1, L = 10, dx = 0.01;
    std::ifstream in("config_file.json");
    assert(in.good());
    json config = json::parse(in);
    in.close();

    double 
        x0 = config["initial coordinate"],
        v0 = config["initial velocity"],
        g  = config["attenuation constant"],
        w  = config["fundamental frequency"],
        L  = config["modeling interspace"],
        dx = config["step of modeling"];
    bool
        write = config["write"];
    std::string
        type = config["model"];
    unsigned
        print_level = config["print level"];


    physical_pendulum v(x0,v0,g,w);
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
}