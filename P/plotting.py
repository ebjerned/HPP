import matplotlib.pyplot as plt

tot_big =  [14.80889, 10.10432, 8.404237, 7.225546, 6.553905, 6.201244, 5.990085, 5.767291, 5.580712, 5.458244, 5.597057, 5.523058, 5.573163, 5.484218, 5.216084, 5.641973]
tot_small = [0.076800, 0.058206, 0.050779, 0.048862, 0.047016, 0.048865, 0.047929, 0.046487, 0.052266, 0.048138, 0.053614, 0.048610, 0.048042, 0.060674, 0.057344, 0.056045]
cores = range(1,17)


size = [27 ,52,102  ,152 ,202 ,402 ]

serial = [0.013205, 0.076965, 0.593701, 1.855697, 4.449759, 35.047610]


c_big  = [tot_big[0]/e for e in cores]
c_small  = [tot_small[0]/e for e in cores]


plt.plot(cores,c_big, "r--")
plt.plot(cores, tot_big, "ro-")
plt.title("Parllel scaling, 300 side cube")
plt.xlabel("No. cores [-]")
plt.ylabel("Time [s]")
plt.legend(["Ideal", "Measured"])
plt.savefig("Parabig.png")

plt.clf()
plt.plot(cores,c_small, "b--")
plt.plot(cores, tot_small, "bo-")
plt.title("Parllel scaling, 50 side cube")
plt.xlabel("No. cores [-]")
plt.ylabel("Time [s]")
plt.legend(["Ideal", "Measured"])
plt.savefig("Parasmall.png")

plt.clf()
plt.semilogy(size,serial, "g--")
plt.grid()
plt.title("Serial complexity")
plt.xlabel("Side length of cube [-]")
plt.ylabel("Time [s]")
plt.legend(["Measured"])
plt.savefig("serial.png")
