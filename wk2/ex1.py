import numpy as np
import matplotlib.pyplot as plt

pi = np.pi

def lissague_data(a, b, delta):
    t = np.linspace(0, 2*pi, 10000, endpoint=False)
    x = np.sin(a * t + delta)
    y = np.sin(b * t)
    points = np.transpose(np.array((x, y)))
    #points = np.unique(points, axis=0)
    return points

def lissague_draw(ax, x, y):
    ax.plot(x, y, "-")

fig, (ax) = plt.subplots(1, 1)
fig.suptitle('Lissague')

points = lissague_data(5, 3, pi/6)
lissague_draw(ax, points[:,0], points[:,1])

plt.show()