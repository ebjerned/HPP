import matplotlib.pyplot as plt
import math
N = [10, 50, 100, 500, 1000, 3000, 5000, 7000, 10000]
N_2 = [e**2 for e in range(1, 100)]
N_3 = [e**3 for e in range(1, 100)]
time = [0.002901, 0.006463, 0.015964, 0.325657, 1.295745, 11.684230, 32.431196, 63.577339, 130.429485]


plt.loglog(N_2, N_2)
plt.loglog(N_3, N_3)
#plt.loglog(N, time, "ro-")
plt.legend(["N^2", "N^3", "Measured time"])
plt.title("Time complexity")
plt.xlabel("Particles [-]")
plt.ylabel("Time [s]")
plt.grid()
plt.savefig("time_complexity2.png")
