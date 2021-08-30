import matplotlib.pyplot as plt
import numpy as np



plt.figure()
plt.plot(range(100), range(100), label = "linear")
plt.plot(range(100), [np.log2(n) for n in range(100)], label = "log2 n")
plt.plot(range(100), [n*np.log2(n) for n in range(100)], label = "n log2 n")
plt.plot(range(100), [n*np.log10(n) for n in range(100)], label = "n log10 n")
plt.legend(loc='best')
plt.show()








