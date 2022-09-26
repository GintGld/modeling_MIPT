import numpy as np
import matplotlib.pyplot as plt

pi = np.pi

def lissague_data(a, b, delta):
    t = np.linspace(0, 2 * pi / min(a, b), 1000)
    x = np.cos(a * t)
    y = np.sin(b * t + delta)
    return x, y

def lissague_draw(ax, x, y):
    ax.plot(x, y, '-')

fig, (ax1, ax2) = plt.subplots(2, 1)
fig.suptitle('A tale of 2 subplots')

x1, y1 = lissague_data(2, 1, pi/4)
x2, y2 = lissague_data(4, 2, pi/3)

lissague_draw(ax1, x1, y1)
lissague_draw(ax2, x2, y2)

plt.show()