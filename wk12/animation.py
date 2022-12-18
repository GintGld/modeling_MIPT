from sympy.physics.mechanics import *
import sympy as sp
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from json import dump
from collections import deque
from subprocess import call

history_len = 500

config = {
    'init' : {
        'q1' : -np.pi/4,
        'q2' : 0,
        'v1' : 0,
        'v2' : 0
    },
    'pars' : {
        'm1' : 1,
        'm2' : 2,
        'l1' : 3,
        'l2' : 4,
        'g' : 9.81
    },
    'modeling' : {
        'time' : 50,
        "dt": .01,
        "steps" :50
    }
}

def make_func(config, val_type = 'double', method = 'rk45'):
    m1, m2, l1, l2, g = sp.symbols('m1 m2 l1 l2 g')
    q1, q2 = dynamicsymbols('s[0] s[1]')
    q1d, q2d = dynamicsymbols('s[0] s[1]', 1)
    v1, v2 = dynamicsymbols('s[2] s[3]')
    L = (m1 + m2)/2 * l1**2 * q1d**2 + m2/2 * l2**2 * q2**2 + m2*l1*l2*q1d*q2d*sp.cos(q1-q2) + (m1 + m2)*g*l1*sp.cos(q1) + m2*g*l2*sp.cos(q2)
    LM = LagrangesMethod(L, [q1, q2])
    LM.form_lagranges_equations()
    out = open('function.cpp', 'w')
    print('#include <valarray>', file=out)
    print('#include <string>', file=out)
    print('#include "function.h"', file=out)
    print('using namespace std;', file=out)
    print('#undef ValType', file=out)
    print('#define ValType '+val_type, file=out)
    print('#undef METHOD', file=out)
    print('#define METHOD (std::string)"'+ method +'"', file=out)
    print('valarray<ValType> F(ValType t, valarray<ValType> s) {', file=out)
    print('\treturn valarray<ValType> {s[2], s[3],', 
        str(LM.forcing[0].subs({
            q1d:v1,
            q2d:v2,
            l1:config['pars']['l1'],
            l2:config['pars']['l2'],
            m1:config['pars']['m1'],
            m2:config['pars']['m2'],
            g:config['pars']['g']})).replace('(t)', ''),
        ', ',
        str(LM.forcing[1].subs({
            q1d:v1,
            q2d:v2,
            l1:config['pars']['l1'],
            l2:config['pars']['l2'],
            m1:config['pars']['m1'],
            m2:config['pars']['m2'],
            g:config['pars']['g']})).replace('(t)', ''),
        '};',
        file=out)
    print('}', file=out)
    out.close()

def make_json(config):
    out = open('config.json', 'w')
    dump({
        'init' : [
            config['init']['q1'],
            config['init']['q2'],
            config['init']['v1'],
            config['init']['v2']
        ],
        'modeling' : {
            'time' : config['modeling']['time'],
            'dt': config['modeling']['dt'],
            'steps' : config['modeling']['steps']
        },
        'pars' : {
        'm1' : config['pars']['m1'],
        'm2' : config['pars']['m2'],
        'l1' : config['pars']['l1'],
        'l2' : config['pars']['l2'],
        'g' : config['pars']['g']
        }
    }, out, indent=4)
    out.close()

make_func(config)
make_json(config)
call('make')
call('main.exe')
data = pd.DataFrame(np.fromfile('output.binary', dtype=np.dtype([
    ('q1', np.float64), 
    ('q2', np.float64), 
    ('v1', np.float64),
    ('v2', np.float64)
    ])))
call('make clean')

L = config['pars']['l1'] + config['pars']['l2']

fig = plt.figure(figsize=(5, 4))
ax = fig.add_subplot(autoscale_on=False, xlim=(-L, L), ylim=(-L-1, 1.))
ax.set_aspect('equal')
ax.grid()

line, = ax.plot([], [], 'o-', lw=2)
trace, = ax.plot([], [], '.-', lw=1, ms=2)
time_template = 'time = %.1fs'
time_text = ax.text(0.05, 0.9, '', transform=ax.transAxes)
history_x, history_y = deque(maxlen=history_len), deque(maxlen=history_len)

t = np.arange(0, config['modeling']['time'], config['modeling']['dt'])
x1 = config['pars']['l1'] * np.sin(data.q1)
y1 =-config['pars']['l1'] * np.cos(data.q1)
x2 = config['pars']['l2'] * np.sin(data.q2) + x1
y2 =-config['pars']['l2'] * np.cos(data.q2) + y1

def animate(i):
    #i = 2*int(i)
    thisx = [0, x1[i], x2[i]]
    thisy = [0, y1[i], y2[i]]

    if i == 0:
        history_x.clear()
        history_y.clear()

    history_x.appendleft(thisx[2])
    history_y.appendleft(thisy[2])

    line.set_data(thisx, thisy)
    trace.set_data(history_x, history_y)
    time_text.set_text(time_template % (i*config['modeling']['dt']))
    return line, trace, time_text

ani = FuncAnimation(fig, animate,
    frames=np.arange(0, int(config['modeling']['time'] / config['modeling']['dt'])),
    blit=True, interval=50)#config['modeling']['dt'] * 1000)

plt.show()