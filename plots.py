import matplotlib.pyplot as plt
from scipy.interpolate import interp1d
import numpy as np


#   PLOTS:    https://www.geeksforgeeks.org/graph-plotting-in-python-set-1/
#INTERPOLATE: https://docs.scipy.org/doc/scipy/reference/tutorial/interpolate.html


def playground():
	x1 = [1,2,3]
	y1 = [2,4,1]

	plt.plot(x1, y1, label = "Algorithm 1")
	  
	x2 = [1,2,3]
	y2 = [4,1,3]

	plt.plot(x2, y2, color="red",label = "Algorithm 2")
	plt.scatter(x2, y2, marker= "*", color="red",label = "Algorithm 3")
	  
	plt.xlabel('x - axis')
	plt.ylabel('y - axis')

	# setting x and y axis range
	#plt.ylim(1,8)
	#plt.xlim(1,8)

	plt.title('Two lines on same graph!')
	plt.legend()

	plt.savefig("teste.jpg")


def plot(stats):

	n 		= [row[0] for row in stats]
	r 		= [row[1] for row in stats]
	time 	= [row[2] for row in stats]
	CALLS 	= [row[3] for row in stats]

	x = np.multiply(np.multiply(n,np.log2(n)),np.sqrt(r))
	y = time

	plt.plot(x, y, label = "Exact Rank - Time")

	plt.plot(x, CALLS, color="red",label = "Exact Rank - Oracle calls")
	  
	plt.xlabel('n log n sqrt(r) T_rank')
	plt.ylabel('Time (s) & Oracle calls')

	# setting x and y axis range
	#plt.ylim(1,8)
	#plt.xlim(1,8)

	plt.title('Sample graph')
	plt.legend()

	plt.savefig("teste.jpg")


file = open("stats.txt", "r")

stats = []
for line in file:
	stats.append(list(map(int, line[:len(line)-1].split(" "))))
	#print(line) [V, S, time microseconds(?) better convert to specific unit of time, ORACLE_CALLS] 

stats = np.array(stats)
print(stats)
#playground()
plot(stats)
