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
    //generic_euler(){}
    T operator()(T st, double dt) const
    {
        return st + st.ev(dt)*dt;
    }
};

template<class T, class gen>
class solver
{
    private:
    std::vector<T> data;
    //double L;
    public:

    solver () {}
    //solver(T init_state, double L): L(L) {data.push_back(init_state);}
    T& operator[](size_t i)
    {
        assert(i >= 0 && i < data.size());
        return this->data[i];
    }
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
        for (size_t i = 0; i < data.size(); ++i)
            out.write((char*)&data[i], sizeof(T));
        out.close();
        cout << "data was written to " << file << endl;
    }
};

int main()
{
    double par=1;
    physical_pendulum v(M_PI-0.01,0,1);
    solver<physical_pendulum, generic_euler<physical_pendulum> > s;
    s.solve(v, 100, 0.001);
    s.write("test.binary");
    s.clear();
    return 0;
}