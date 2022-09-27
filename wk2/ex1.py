import numpy as np
import matplotlib.pyplot as plt


pi = np.pi
err = np.array([None,None], dtype='float')

def intersection(p1, p2, q1, q2):
    '''
        function check, does to segments intersect.
        return:  intersection pont w if yes,
                err (np.array (1,2)) if no.

    '''
    a1 = np.array([p1[1] - p2[1], p2[0] - p1[0]])
    a2 = np.array([q1[1] - q2[1], q2[0] - q1[0]])
    w = np.linalg.solve([a1,a2], [np.inner(p1,a1),np.inner(q1,a2)])
    if (
        np.inner(p1-w,p2-w) < 0
    )and(
        np.inner(q1-w,q2-w) < 0
    ): return w
    return err

class lissague:
    '''
        Class that involve all information about lissague 
        figure with given parameters (A, B, Delta, N).
    '''
    def __init__(self, A, B, Delta, N, cv, cv_hist):
        '''
            constructor
            cv - canvas for drawing figure
            cv_hist - canvas for histogramm
            A, B, Delta - figure parameters
            N - amount of steps
            if A, B aren't big (<=7)  N >= 500
            points - figure coordinates (x(t), y(t), t).
            int_p - intersection points' coordinates (x, y).
            int_t - pair of t values in intersection points.
        '''
        self.cv = cv
        self.cv_hist = cv_hist
        self.cv.set_title('A='+str(A)+', B='+str(B)+', $\delta$='+str(round((Delta/pi), 2))+'$\pi$')

        self.t = np.linspace(0, 2 * pi, N)
        self.points = np.transpose(np.array(( #массив точек
            np.sin(A * self.t + Delta), # x
            np.sin(B * self.t),         # y
            self.t)))                   # t
        x = []
        y = []
        t = []
        print('Done:')
        for i in range(0, N-2, 1):
            if ( i % (N / 10) == 0): print(str(int((10 * i) / (N / 10))) + '%')
            for j in range(N-2, i+2, -1):
                if (i != j):
                    w = intersection([
                        self.points[i][0], self.points[i][1]], [
                        self.points[i+1][0], self.points[i+1][1]],[
                        self.points[j][0], self.points[j][1]], [
                        self.points[j+1][0], self.points[j+1][1]])
                    if np.logical_not(np.array_equal(w, err)): 
                        x.append(w[0])
                        y.append(w[1])
                        t.append([i, j])
        self.int_p = np.transpose(np.array([x,y]))
        self.int_t = np.array(t)
        print('100%')

    def draw(self):
        '''
            draws lissague figure 
            and intersection points.
            ax - given canvas
        '''
        self.cv.set_xlim(-1.05,1.05)
        self.cv.set_ylim(-1.05,1.05)
        self.cv_hist.set_xlim(-1.05,1.05)
        self.cv.plot(self.points[:,0], self.points[:,1], '-')
        self.cv.scatter(self.int_p[:,0], self.int_p[:,1], s=30, c='black', marker='D')
        self.cv_hist.hist(self.int_p[:,0], bins=101, histtype='step', align='mid'), 


#plt.figure(figsize=(14, 16), dpi=80)
fig, ax = plt.subplots(2, 3)
fig.suptitle('Lissague')

L = np.array([
    lissague(5, 3, pi/11, 500, ax[0][0], ax[1][0]),
    lissague(3, 7, pi/3, 800, ax[0][1], ax[1][1]),
    lissague(9, 8, pi/4, 700, ax[0][2], ax[1][2]),
], dtype=lissague)
for l in L: l.draw()



fig.tight_layout()
plt.show()