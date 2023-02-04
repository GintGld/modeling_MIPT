#ifndef PHYSICS_H
#define PHYSICS_H

#include <array>
#include <cmath>

using std::size_t;

template<typename MF, size_t phase_size, size_t pars_size>
class state
{
private:
    std::array<MF, phase_size> phase;
    std::array<MF, pars_size> pars;

public:
    state()
    {
        for(int i = 0; i < phase_size; ++i)
            phase[i] = 0;
        for(int i = 0; i < pars_size; ++i)
            pars[i] = 0;
    }
    state(std::array<MF, phase_size> phase, std::array<MF, pars_size> pars):
        phase(phase), pars(pars) {}

    state operator=(state other)
    {
        phase = other.phase;
        pars = other.pars;
        return *this;
    }
    state operator+(state other)
    {
        state tmp(phase, pars);
        for (int i = 0; i < phase_size; ++i)
            tmp.phase[i] += other.phase[i];
        return tmp;
    }
    state operator-(state other)
    {
        state tmp(phase, pars);
        for (int i = 0; i < phase_size; ++i)
            tmp.phase[i] -= other.phase[i];
        return tmp;
    }
    state operator*(MF d)
    {
        state tmp(phase, pars);
        for (int i = 0; i < phase_size; ++i)
            tmp *= d;
        return tmp;
    }

    template<typename MF1, size_t N1, size_t N2>
    friend state<MF1, N1, N2> operator*(MF1 d, state<MF1, N1, N2> st);
};

template<typename MF1, size_t N1, size_t N2>
state<MF1, N1, N2> operator*(MF1 d, state<MF1, N1, N2> st)
{
    state<MF1, N1, N2> tmp(st.phase, st.pars);
    for (int i = 0; i < N1; ++i)
        tmp.phase[i] *= d;
    return tmp;
}

template<typename MF>
class abstract
{
protected:
    size_t phase_size, pars_size;
    state<MF, phase_size, pars_size> st;
public:
    abstract(state<MF, phase_size, pars_size> st)
        st(st) {}
};

template<typename MF>
abstract<MF> diff(abstract<MF>);

template<typename MF>
class math_harm : abstract<MF>
{
    phase_size = 2; pars_size = 4;
};

template<typename MF>
class phys_harm : abstract<MF>
{
    phase_size = 2; pars_size = 4;
};

template<typename MF>
math_harm<MF> diff(math_force<MF> st, MF time)
{
    std::array<MF, 2> phase;
    phase[0] = st.phase[1];
    phase[1] = st.pars[2] * cos(st.pars[3] * time) - st.pars[1] * st.pars[1] * st.phase[0] - 2 * st.pars[0] * st.phase[1];
    return state<MF, 2, 4>(phase, st.pars);
}

template<typename MF>
phys_harm<MF> diff(math_free<MF> st, MF time)
{
    std::array<MF, 2> phase;
    phase[0] = st.phase[1];
    phase[1] = st.pars[2] * cos(st.pars[3] * time) - st.pars[1] * st.pars[1] * sin(st.phase[0]) - 2 * st.pars[0] * st.phase[1];
    return state<MF, 2, 4>(phase, st.pars);
}


#endif // PHYSICS_H

/*class physical_pendulum
{
    
        x'' + 2 g x' + w_0^2 sin(x) = A cos(w t)

        phase = {x, x'}
        pars  = {g, w0, A, w}
    
private:
    std::array<double, 2> phase;
    std::array<double, 4> pars;
    size_t phase_size = 2, pars_size = 4;
public:
    physical_pendulum()
    {
        for (size_t i = 0; i < phase_size; ++i)
            phase[i] = 0;
        for (size_t i = 0; i < pars_size; ++i)
            pars[i] = 0;
    }
    physical_pendulum(double x1, double x2, double x3, double x4, double x5, double x6)
    {
        phase[0]=x1; phase[1]=x2; pars[0]=x3; pars[1]=x4; pars[2] = x5; pars[3] = x6;
    }

    void operator=(physical_pendulum other)
    {
        phase = other.phase; pars = other.pars;
    }
    physical_pendulum operator+(physical_pendulum other)
    {
        assert(pars == other.pars);
        return physical_pendulum(phase[0]+other.phase[0], phase[1]+other.phase[1], pars[0], pars[1], pars[2], pars[3]);
    }
    physical_pendulum operator*(double d)
    {
        return physical_pendulum(phase[0]*d, phase[1]*d, pars[0], pars[1], pars[2], pars[3]);
    }
    physical_pendulum ev(double time)
    {
        return physical_pendulum(phase[1], pars[2] * cos(pars[3] * time) - 2 * pars[0] * phase[1] - pars[1] * pars[1] * sin(phase[0]), pars[0], pars[1], pars[2], pars[3]);
    }
    std::array<double,2> get_phase()
    {
        return phase;
    }
};*/