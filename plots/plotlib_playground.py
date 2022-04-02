import numpy as np
from matplotlib import pyplot as plt
import math

sapf = open("sap.txt", "r")
cunf = open("cun.txt", "r")

sap = []
cun = []

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

n1 		 = [row[0] for row in sap]
r1 		 = [row[1] for row in sap]
CALLS1 	 = [row[2] for row in sap]
O_CALLS1 = [row[3] for row in sap]
time1 	 = [row[4] for row in sap]	
bound1   = [row[0]*row[1]*row[1] for row in sap]

n2 		 = [row[0] for row in cun]
r2 		 = [row[1] for row in cun]
CALLS2   = [row[2] for row in cun]
O_CALLS2 = [row[3] for row in cun]
time2	 = [row[4] for row in cun]	
bound2   = [row[0]*row[1]*math.sqrt(row[1]) for row in cun]

if (len(n1)!=len(n2) or len(r1)!=len(r2) or len(CALLS1)!=len(CALLS2) or len(O_CALLS1)!=len(O_CALLS2) or len(bound1)!=len(bound2)):
	print(">>ERROR")
	exit()
for i in range(len(n1)):
	if (n1[i]!=n2[i] or r1[i]!=r2[i]):
		print(">>ERROR")
		exit()

####################

X = np.array(bound2)

y = np.array(CALLS1)
z = np.array(CALLS2)

# Plotting both the curves simultaneously
plt.plot(X, y, color='r', label='A&D')
plt.plot(X, z, color='g', label='Cunningham')
  
# Naming the x-axis, y-axis and the whole graph
plt.xlabel("nr^1.5")
plt.ylabel("Number of queries")
plt.title("Title")

#Adding text at coordinate 8x,y)
#plt.text(60, .025, r'$\mu=100,\ \sigma=15$')

plt.xlim(0, max(bound2)+200)
#plt.ylim(min(min(time1),min(time2)), max(max(time1),max(time2)))
plt.ylim(min(min(CALLS1),min(CALLS2)), max(max(CALLS1),max(CALLS2)))

# Adding legend, which helps us recognize the curve according to it's color
plt.legend()

#Adding a grid
plt.grid(True, color='grey', ls=':', lw=1, zorder=1)
#plt.grid(True)
  
# To load the display window
plt.savefig('CALLSx.svg')


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