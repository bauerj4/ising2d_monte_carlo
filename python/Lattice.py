import numpy as np
import sys
import matplotlib.pylab as plt

nprocs = int(sys.argv[1])
lattice = int(sys.argv[2])
data = []
dt = np.dtype("int16")
dim = 0
for i in range(0, nprocs):
    thisSnap = np.fromfile("OUTPUTS/ising." + str(i), dtype=dt)
    dim = len(thisSnap)
    print dim
    data = data + list(thisSnap)

print data
i = 0
for x in data:
    if (x != -1 and x != 1):
        data[i] = 1
    i = i + 1
data = np.array(data)
data = np.reshape(10**data, (lattice, lattice))

plt.imshow(data, interpolation=None)
plt.show()
