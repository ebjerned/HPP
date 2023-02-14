import matplotlib.pyplot as plt
import math
N = [10, 50, 100, 500, 1000, 3000, 5000, 7000, 10000]
N_2 = [100, 10000**2]
N_3 = [1000, 10**12]
time = [0.002901, 0.006463, 0.015964, 0.325657, 1.295745, 11.684230, 32.431196, 63.577339, 130.429485]


plt.loglog([10, 10000], N_2)
plt.loglog([10, 10000], N_3)
plt.loglog(N, time, "ro-")
plt.legend(["N^2", "N^3", "Measured time"])
plt.title("Time complexity")
plt.xlabel("Particles [-]")
plt.ylabel("Time [s]")
plt.grid()
plt.savefig("time_complexityyyy.png")
