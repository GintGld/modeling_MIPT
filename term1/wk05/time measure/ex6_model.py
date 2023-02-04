import numpy as np
import json

class state:
    def __init__(self, x, v, w):
        self.x = x
        self.v = v
        self.w = w
    def __add__(self, o):
        return state (self.x+o.x, self.v+o.v, self.w)
    def __mul__(self, a):
        return state (self.x * a.x, self.v * a.v, self.w)
    def print(self):
        print('('+str(self.x)+', '+str(self.v)+', '+str(self.w)+')')

def f(y, x):
    return state (y.v, -y.w ** 2 * x, y.w)

class Solver:
    def __init__(self, w, x0, v0, L, h, model, file):
        self.w = w
        self.x0 = x0
        self.v0 = v0
        self.L = L
        self.h = h
        self.N = int(L/h)
        self.model = model
        self.file = file
        self.data = np.zeros(self.N, dtype=state)
        self.data[0] = state (x0, v0 , w)
        self.index = 0
    def add_step(self):
        if (self.index + 1 < self.N):
            y = self.data[self.index]
            if (self.model == 'Euler'):
                self.data[self.index+1] = y + f(y, y.x) * state(self.h,self.h,0)
            if (self.model == 'Heun'):
                k1 = f(y, y.x)
                k2 = f(y + k1 * state(self.h,self.h,0), y.x + self.h * k1.x)
                self.data[self.index+1] = y + (k1 + k2) * state(0.5*self.h,0.5*self.h,0)
            if (self.model == 'RK45'):
                k1 = f(y, y.x)
                k2 = f(y + k1 * state(0.5*self.h,0.5*self.h,0), y.x + 0.5 * self.h)
                k3 = f(y + k2 * state(0.5*self.h,0.5*self.h,0), y.x + 0.5 * self.h)
                k4 = f(y + k3 * state(0.5*self.h,0.5*self.h,0), y.x + self.h)
                self.data[self.index+1] = y + (k1 + k2*state(2,2,0) + k3*state(2,2,0) + k4) * state(self.h/6,self.h/6,0) 
            self.index += 1
    def solve(self):
        for i in range(self.N):
            self.add_step()
            if (i == int(float(self.N)*0.1)-1): print('10% solved')
            if (i == int(float(self.N)*0.2)-1): print('20% solved')
            if (i == int(float(self.N)*0.3)-1): print('30% solved')
            if (i == int(float(self.N)*0.4)-1): print('40% solved')
            if (i == int(float(self.N)*0.5)-1): print('50% solved')
            if (i == int(float(self.N)*0.6)-1): print('60% solved')
            if (i == int(float(self.N)*0.7)-1): print('70% solved')
            if (i == int(float(self.N)*0.8)-1): print('80% solved')
            if (i == int(float(self.N)*0.9)-1): print('90% solved')
            if (i == int(float(self.N)*1)-1): print('100% solved')
    def print(self):
        print('type: Solver\nparameters:\nmodel\t\t', self.model)
        print('init. coord\t', self.x0)
        print('init. velocity\t', self.v0)
        print('omega\t\t', self.w)
        print('mod. interspace\t', self.L)
        print('step of count\t', self.h, '\n')
    def write_data(self):
        print('collecting files')
        out = open(self.file, 'w')
        print('x,v,w,L,h', file=out)
        for v in self.data:
            print(
                str(v.x)+','+
                str(v.v)+','+
                str(self.w)+','+
                str(self.L)+','+
                str(self.h), file = out)
        out.close()


read = open('config_file.json', 'r')
config = json.load(read)
read.close()

N = len(config['entries'])
Solvers = np.zeros(N, dtype=Solver)

print('Number of entries', N, '\n')

for i in range(N):
    Solvers[i] = Solver (
        config['entries'][i]['omega'],
        config['entries'][i]['initial coordinate'],
        config['entries'][i]['initial velocity'],
        config['entries'][i]['modeling interspace'],
        config['entries'][i]['step of modeling'],
        config['entries'][i]['model'],
        config['entries'][i]['file name'])

for i in range(N):
    Solvers[i].print()
    Solvers[i].solve()
    Solvers[i].write_data()