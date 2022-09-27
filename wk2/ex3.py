import numpy as np
import matplotlib.pyplot as plt


A = 2
B = 0.64
mu, sigma = 0, 3

L = 10
N = 11

fig, ax = plt.subplots()

x = 20*np.random.rand(N)
y = np.random.normal(mu, sigma, N) + A * x + B

p = np.polyfit(x, y, deg=1)
f = np.poly1d(p)

t = np.linspace(min(x), max(x), 10)
ax.plot(t, f(t))
ax.scatter(x, y, c='tab:brown')

plt.show()