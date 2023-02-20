import matplotlib.pyplot as plt
import numpy as np

t= np.loadtxt("speedup.txt")

s = [e/t[0][0] for e in t[0]];

plt.plot(t[0],s)
plt.xlabel("Number of threads");
plt.ylabel("Performance increase");
plt.savefig("openmp_speedup");
