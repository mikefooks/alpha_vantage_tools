import numpy as np
from matplotlib import pyplot as plt

'''
diffs = open("generated.dat", "r").read()
diffs = diffs.split('\n')[:-1]
diffs = np.array([ float(val) for val in diffs ])
diffs = diffs.reshape((184, 11))
means = np.mean(diffs, axis=1)
'''

generated_diffs = open("generated.dat", "r").read()
generated_diffs = [ float(val) for val in generated_diffs.split()[:-1] ]


cur_price = 230.77

forecast = []
for d in generated_diffs:
    cur_price += (cur_price * d)
    forecast.append(cur_price)


plt.plot(forecast)
plt.show()
