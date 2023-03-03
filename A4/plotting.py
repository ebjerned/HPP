import matplotlib.pyplot as plt
import numpy as np

t= np.loadtxt("A4/Openmp/speedup.txt")
t2= np.loadtxt("../A4_additional/Pthreads/speedup.txt")

threads = [e[0] for e in t]
s = [t[0][1]/e[1] for e in t]
s2 = [t2[0][1]/e[1] for e in t2]


plt.plot(threads,s, "ro-", markerfacecolor='none')
plt.plot(threads,s2, "go-",markerfacecolor='none')
plt.plot(threads, threads)
plt.legend(["OpenMP", "Pthread", "Ideal"])
plt.xlabel("Number of threads[-]")
plt.ylabel("Performance increase [times]")
plt.title("Comparison of methods")
plt.grid()
plt.savefig("Comparison of methods.png")
