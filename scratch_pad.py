import numpy as np
from matplotlib import pyplot as plt


diffs = open("diffs.dat", "r").read()
diffs = diffs.split('\n')[:-1]
diffs = np.array([ float(val) for val in diffs ])
diffs = diffs.reshape((184, 11))

means = np.mean(diffs, axis=1)

plt.plot(means)
plt.show()
