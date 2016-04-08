import numpy as np
import matplotlib.pylab as plt

dt = np.dtype(float)

Temperatures = np.fromfile("OUTPUTS/temperature.bin",dtype=dt)
Energies = np.fromfile("OUTPUTS/energy.bin",dtype=dt)

plt.plot(Temperatures, Energies)
plt.show()
