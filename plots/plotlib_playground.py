import numpy as np
from matplotlib import pyplot as plt
import math

sapf   = open("stats/sap.txt", "r")
cunf   = open("stats/cun.txt", "r")
indepf = open("stats/indep.txt", "r")
rankf  = open("stats/rank.txt", "r")

sap   = []
cun   = []
indep = []
rank  = []

i=0
for line in sapf:
	if i!=0:
		sap.append(list(map(int, line[:len(line)-1].split(" "))))
	i+=1
i=0
for line in cunf:
	if i!=0:
		cun.append(list(map(int, line[:len(line)-1].split(" "))))
	i+=1
i=0
for line in indepf:
	if i!=0:
		indep.append(list(map(int, line[:len(line)-1].split(" "))))
	i+=1
i=0
for line in rankf:
	if i!=0:
		rank.append(list(map(int, line[:len(line)-1].split(" "))))
	i+=1

n1 		 = [row[0] for row in sap]
r1 		 = [row[1] for row in sap]
CALLS1 	 = [row[2] for row in sap]
O_CALLS1 = [row[3] for row in sap]
time1 	 = [row[4] for row in sap]
bound_S  = [row[0]*row[1]*row[1] for row in sap]

n2 		 = [row[0] for row in cun]
r2 		 = [row[1] for row in cun]
CALLS2   = [row[2] for row in cun]
O_CALLS2 = [row[3] for row in cun]
time2	 = [row[4] for row in cun]	
bound_C  = [row[0]*row[1]*math.sqrt(row[1]) for row in cun]

n3 		 = [row[0] for row in indep]
r3 		 = [row[1] for row in indep]
CALLS3 	 = [row[2] for row in indep]
O_CALLS3 = [row[3] for row in indep]
time3 	 = [row[4] for row in indep]	
bound_I  = [row[0]*row[1]*math.log(row[1],2) for row in indep]

n4 		 = [row[0] for row in rank]
r4 		 = [row[1] for row in rank]
CALLS4   = [row[2] for row in rank]
O_CALLS4 = [row[3] for row in rank]
time4	 = [row[4] for row in rank]	
bound_R  = [row[0]*math.log(row[0],2)*math.sqrt(row[1]) for row in rank]


#TODO error checking
'''
if (len(n1)!=len(n2) or len(r1)!=len(r2) or len(CALLS1)!=len(CALLS2) or len(O_CALLS1)!=len(O_CALLS2) or len(bound1)!=len(bound2)):
	print(">>ERROR")
	exit()
for i in range(len(n1)):
	if (n1[i]!=n2[i] or r1[i]!=r2[i]):
		print(">>ERROR")
		exit()
'''

####################

fig = plt.figure()
ax = fig.add_subplot(1, 1, 1)

X = np.array(bound_C)

y = np.array(O_CALLS1)
z = np.array(O_CALLS2)
w = np.array(O_CALLS3)
o = np.array(O_CALLS4)

xmax=max(bound_C)
ymax=max(CALLS2)
#ymax=max(max(O_CALLS1),max(O_CALLS2),max(O_CALLS3),max(O_CALLS4))
plt.xlim(0, xmax)
plt.ylim(0, ymax)
#plt.ylim(min(min(time1),min(time2)), max(max(time1),max(time2)))


# Adding a grid
major_ticks = ax.get_xticks()
minor_ticks = np.array( [ major_ticks[i]+(major_ticks[i+1]-major_ticks[i])//2 for i in range(len(major_ticks)-1)] )
ax.set_xticks(major_ticks)
ax.set_xticks(minor_ticks, minor=True)

major_ticks = ax.get_yticks()
minor_ticks = np.array( [ major_ticks[i]+(major_ticks[i+1]-major_ticks[i])//2 for i in range(len(major_ticks)-1)] )
ax.set_yticks(major_ticks)
ax.set_yticks(minor_ticks, minor=True)

ax.grid(which='both')

#https://stackoverflow.com/questions/24943991/change-grid-interval-and-specify-tick-labels-in-matplotlib
#https://matplotlib.org/3.5.0/api/_as_gen/matplotlib.pyplot.grid.html

ax.grid(which='major', alpha=0.7, color='grey', linestyle=':', linewidth=1)
ax.grid(which='minor', alpha=0.9, color='grey', linestyle=':', linewidth=0.6)
#ax.grid(which='major', color='grey', linestyle=':', linewidth=1, alpha=0.5)

#OLD
#plt.grid(True, color='grey', ls=':', lw=1)

# Plotting both the curves simultaneously
#plt.plot(X, y, 'rs-', linewidth=1, markersize=3.5, label='A&D')
plt.plot(X, z, 'go-', linewidth=1, markersize=4, label='Cunningham') #https://www.geeksforgeeks.org/graph-plotting-in-python-set-1/
#plt.plot(X, w, 'b^-', linewidth=1, markersize=4, label='Indep')
#plt.plot(X, o, 'y*--', linewidth=1, markersize=5, label='ExactRank')
#(x, y, color='green', linestyle='dashed', linewidth = 3, marker='o', markerfacecolor='blue', markersize=12)

#plt.plot(X, X, 'k-', linewidth=1)	

# Naming the x-axis, y-axis and the whole graph
plt.xlabel("$n ~ r ~ \log r$")
plt.ylabel("Number of queries")
plt.title("Title???")

#Adding text at coordinate 8x,y)
#plt.text(60, .025, r'$\mu=100,\ \sigma=15$')

# Adding legend, which helps us recognize the curve according to it's color
plt.legend()

plt.savefig('results/testing.svg')


"""
Generates one png image with two graphs, one plot in each

# Preparing the data for the plot
x = np.arange(0.0, 10.0, 0.005)
y = np.exp(2.3 * x + 3.7)

# Creating figure and axes
fig, (ax1, ax2) = plt.subplots(2, 1, figsize=[7, 11])

# Plotting the graph without using loglog
ax1.plot(x, y, ':b', linewidth=2, label='e ^ (2.3 * x + 3.7)')
ax1.set_title('Exponential plot', fontsize=15)
ax1.set_xlabel('x-axis', fontsize=13)
ax1.set_ylabel('y-axis', fontsize=13)
ax1.legend()

# Plotting the graph with Log ticks at x and y axis using loglog
ax2.loglog(x, y, '--r', linewidth=2, label='e ^ (2.3 * x + 3.7)')
ax2.set_title('loglog exponential plot', fontsize=15)
ax2.set_xlabel('log(x)', fontsize=13)
ax2.set_ylabel('log(y)', fontsize=13)
ax2.legend()

plt.grid(True, color='black', ls=':', lw=1, zorder=1)

plt.tight_layout()
plt.savefig('foo2.png')
"""


"""
sin and cos example

# Using Numpy to create an array X
X = np.arange(0, math.pi*2, 0.05)
  
# Assign variables to the y axis part of the curve
y = np.sin(X)
z = np.cos(X)

#read file here. define input std like: solution_size total_oracle_calls time_seconds
  
# Plotting both the curves simultaneously
plt.plot(X, y, color='r', label='sin')
plt.plot(X, z, color='g', label='cos')
  
# Naming the x-axis, y-axis and the whole graph
plt.xlabel("Angle")
plt.ylabel("Magnitude")
plt.title("Sine and Cosine functions")
  
# Adding legend, which helps us recognize the curve according to it's color
plt.legend()
plt.grid(True, color='grey', ls=':', lw=1, zorder=1)
  
# To load the display window
plt.savefig('foo3.png')


"""