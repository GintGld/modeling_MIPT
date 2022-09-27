import numpy as np
import matplotlib.pyplot as plt


pi = np.pi

class lissague():
    def __init__(self, A, B, Delta, N):
        self.t = np.linspace(0, 2 * pi, N)
        x = np.sin(A * self.t + Delta)
        y = np.sin(B * self.t)
        self.points = np.transpose(np.array((
            np.sin(A * self.t + Delta), # x
            np.sin(B * self.t),         # y
            self.t)))                   # t
        

    def draw_line(self, ax):
        ax.plot(self.points[:,0], self.points[:,1], '-')
    
    def draw_scatter(self, ax):
        ax.scatter(self.points[:,0], self.points[:,1], s=2)

fig, (ax) = plt.subplots(1, 1)
fig.suptitle('Lissague')

l1 = lissague(5, 3, pi/11, 5000)
l1.draw_scatter(ax)

plt.show()