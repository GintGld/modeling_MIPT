import numpy as np
from scipy import linalg


'''
    n <= m - size of coeffitient matrix M
    b - free-coeffitient column
    x = pseudo-inverse(M) * b - solution
'''

n = 15
m = 20
step = 8

def cond(x, y):
    if x >=0 and x < n and y >= 0 and y < m: return True
    else: return False

M = np.zeros((n, m), dtype='float')
for i in range(0, n, 1):
    for j in range(i, m, step):
        if cond(i, j): M[i][j] = 2
        if cond(i, j+1): M[i][j+1]=-1
        if cond(i, j-1): M[i][j-1]=-1
        if cond(i-1, j): M[i-1][j]=-1
        if cond(i+1, j): M[i+1][j]=-1

b = np.zeros((n, 1), dtype='float')
b[0] = 1
b[n - 1] = 1

x = np.matmul(linalg.pinv(M), b)

print("Solution x = pseudo-inverse(M) * b:")
print(x)
print("check: Mx - b = ?")
print(np.matmul(M, x) - b)