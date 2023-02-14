import matplotlib.pyplot as plt

N = [10, 50, 100, 500, 1000, 5000, 10000]
N_2 = [e**2 for e in range(1, 10000, 100)]
t = range(1, 10000, 100)
time = [0.002901, 0.006463, 0.015964, 0.325657, 1.295745, 32.431196, 130.429485]

#plt.loglog(N_2, t)
plt.loglog(N, time)
plt.legend(["N^2", "Measured time"])
plt.title("Time complexity")
plt.xlabel("Particles [-]")
plt.ylabel("Time [s]")
plt.grid()
plt.savefig("time_complexity.png")
