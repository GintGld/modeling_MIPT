#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <cassert>

using std::cout;
using std::endl;
using std::size_t;

class physical_pendulum
{
    /*
        M = eJ= -mg sin(phi) уравнение движения
        phi'' = - (mg/J) * sin(phi)
        par := mg / J
    */
public:

    double phi, w, par;

    physical_pendulum(): phi(0), w(0), par(0) {}
    physical_pendulum(double phi, double w, double par): phi(phi), w(w), par(par) {}

    void operator=(physical_pendulum other)
    {
        phi=other.phi;
        w=other.w;
    }
    physical_pendulum operator+(physical_pendulum other)
    {
        assert(par==other.par);
        return physical_pendulum(phi+other.phi, w+other.w, par);
    }
    physical_pendulum operator*(double d)
    {
        return physical_pendulum(phi * d, w * d, par);
    }
    physical_pendulum ev(double dt)
    {
        return physical_pendulum(w, -par * sin(phi), par);
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
    public:

    solver () {}
    T& operator[](size_t i)
    {
        assert(i >= 0 && i < data.size());
        return this->data[i];
    }
    template<class gen>
    void solve(T init_state, double L, double dt)
    {
        gen f;
        data.push_back(init_state);
        for(size_t i = 1; i < size_t(L/dt); ++i)
        {
            data.push_back(f(data[data.size()-1], dt));
            if ((i+1) % (size_t(L/dt) / 10) == 0) cout << 10*((i+1) / (size_t(L/dt) / 10)) << "\% done\n";
        }
        cout << "modeling done\n";
    }
    void clear() {data.clear();}
    void write(std::string file)
    {
        std::ofstream out(file, std::ios::binary);
        assert(out.good());
        for (size_t i = 0; i < data.size(); ++i)
            out.write((char*)&data[i], sizeof(T));
        out.close();
        cout << "data was written to " << file << endl;
    }
};

int main(int argc, char* argv[])
{
    double x0 = M_PI_4, v0 = 0, par = 1, L = 10, dx = 0.01;
    std::string type = "euler", write = "write";
    if (argc < 8)
    {
        std::cout << "not enough paramteres";
        return 1;
    }
    x0  = std::stod(argv[1]);
    v0  = std::stod(argv[2]);
    par = std::stod(argv[3]);
    L   = std::stod(argv[4]);
    dx  = std::stod(argv[5]);
    type = argv[6];
    write = argv[7];

    physical_pendulum v(x0,v0,par);
    solver<physical_pendulum> s;

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
    
    if (write == "write")
        s.write("modeled_data.binary");
    s.clear();
    return 0;
}