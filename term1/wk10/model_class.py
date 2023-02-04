import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import subprocess
import json

class model:
    def __init__(self, x, v, g, w0, A, w, type, L, dx):
        self.config = {
            'model' : type,
            'initial coordinate' : x,
            'initial velocity' : v,
            'attenuation constant' : g,
            'fundamental frequency' : w0,
            'force amplitude': A,
            'force frequency': w,
            'modeling interspace' : L,
            'step of modeling' : dx,
            'write' : True,
            'print level' : 0
        }
        self.is_data = False
        self.is_energy = False
        return
    def data(self):
        write = open('config_file.json', 'w')
        json.dump(self.config, write, indent=4)
        write.close()
        returned = subprocess.check_output('m.exe')
        print(returned.decode('utf-8'))
        self.data =  pd.DataFrame(np.fromfile('modeled_data.binary', dtype=np.dtype([
                ('x', np.float64), 
                ('v', np.float64), 
            ])))
        self.is_data = True
        return
    def energy(self):
        self.energy = ((0.5 / self.config['fundamental frequency'] ** 2) * self.data.v ** 2 + 1 - np.cos(self.data.x)) / ((0.5 / self.config['fundamental frequency'] ** 2) * self.data.v[0] ** 2 + 1 - np.cos(self.data.x[0]))
        self.is_energy = True
        return
    def draw_data(self, ax):
        if self.is_data == False:
            self.data()
        t = np.linspace(
            0, self.config['modeling interspace'], 
            int(self.config['modeling interspace'] / self.config['step of modeling']))
        ax.plot(t, self.data.x)
        return
    def draw_energy(self, ax):
        if self.is_data == False:
            self.data()
        t = np.linspace(
            0, self.config['modeling interspace'], 
            int(self.config['modeling interspace'] / self.config['step of modeling']))
        ax.plot(t, self.energy)
        return
    def draw_phase(self, ax):
        if self.is_data == False:
            self.data()
        ax.plot(self.data.x, self.data.v)
        return

def find_amplitude(x, v, g, w0, A, w):
    tmp = model(x, v, g, w0, A, w, 'rk', 10 / g, 0.01)
    tmp.data()
    d = tmp.data.x[int(0.9 * len(tmp.data)):]
    return d.max() - d.min()

if __name__=='__main__':
    print('This is not main file')