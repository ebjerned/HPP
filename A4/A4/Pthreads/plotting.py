import matplotlib.pyplot as plt
import numpy as np

t= np.loadtxt("speedup.txt")

threads = [e[0] for e in t]
s = [t[0][1]/e[1] for e in t]


plt.plot(threads,s, "ro")
plt.plot(threads, threads)
plt.xlabel("Number of threads")
plt.ylabel("Performance increase")
plt.savefig("openmp_speedup")
