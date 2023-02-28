import matplotlib.pyplot as plt
import numpy as np

t= np.loadtxt("A4/Openmp/speedup.txt")
t2= np.loadtxt("A4/Pthreads/speedup.txt")

threads = [e[0] for e in t]
threadsspeedup = [1/e[0] for e in t]
s = [e[1] for e in t]
s2 = [e[1] for e in t2]


plt.plot(threads,s, "ro-")
plt.plot(threads,s, "go-")
plt.plot(threads, threadsspeedup)
plt.legend(["OpenMP", "Pthread", "Ideal"])
plt.xlabel("Number of threads")
plt.ylabel("Run time [s]")
plt.savefig("Comparison of methods")
