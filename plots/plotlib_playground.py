import numpy as np
from matplotlib import pyplot as plt
import math

sapf   = open("stats/sap_blocks2.txt", "r")
cunf   = open("stats/cun_blocks2.txt", "r")
indepf = open("stats/indep_blocks2.txt", "r")
rankf  = open("stats/rank_blocks2.txt", "r")

ekf = open("stats/ek_blocks2.txt", "r")
hkf = open("stats/hk_blocks2.txt", "r")

sap   = []
cun   = []
indep = []
rank  = []

ek = []
hk = []

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
i=0
for line in ekf:
	if i!=0:
		ek.append(list(map(int, line[:len(line)-1].split(" "))))
	i+=1
i=0
for line in hkf:
	if i!=0:
		hk.append(list(map(int, line[:len(line)-1].split(" "))))
	i+=1



v5 		 = [2*row[0]+row[1] for row in ek]
g5		 = [row[1] for row in ek]
e5  	 = [row[2] for row in ek]
r5  	 = [row[3] for row in ek]
time5 	 = [row[4] for row in ek]

v6 		 = [2*row[0] for row in hk]
g6		 = [row[1] for row in hk]
e6  	 = [row[2] for row in hk]
r6  	 = [row[3] for row in hk]
time6 	 = [row[4] for row in hk]

n1 		 = [row[0] for row in sap]
r1 		 = [row[1] for row in sap]
CALLS1 	 = [row[2] for row in sap]
O_CALLS1 = [row[3] for row in sap]
time1 	 = [row[4] for row in sap]
bound_S  = [row[0]*row[1]*row[1] for row in sap]
exch_S 	 = [row[5] for row in sap]

n2 		 = [row[0] for row in cun]
r2 		 = [row[1] for row in cun]
CALLS2   = [row[2] for row in cun]
O_CALLS2 = [row[3] for row in cun]
time2	 = [row[4] for row in cun]	
bound_C  = [row[0]*row[1]*math.sqrt(row[1]) for row in cun]
exch_C 	 = [row[5] for row in cun]

n3 		 = [row[0] for row in indep]
r3 		 = [row[1] for row in indep]
CALLS3 	 = [row[2] for row in indep]
O_CALLS3 = [row[3] for row in indep]
time3 	 = [row[4] for row in indep]	
bound_I  = [row[0]*row[1]*math.log(row[1],2) for row in indep]
exch_I 	 = [row[5] for row in indep]

n4 		 = [row[0] for row in rank]
r4 		 = [row[1] for row in rank]
CALLS4   = [row[2] for row in rank]
O_CALLS4 = [row[3] for row in rank]
time4	 = [row[4] for row in rank]	
bound_R  = [row[0]*math.log(row[0],2)*math.sqrt(row[1]) for row in rank]
exch_R 	 = [row[5] for row in rank]


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

X = np.array(v5)

y = np.array(time1)
z = np.array(time2)
w = np.array(time3)
o = np.array(time4)
q = np.array(time5)
p = np.array(time6)


xmin=min(v5)
xmax=max(v5)
ymin=min(min(time1),min(time2),min(time3),min(time4),min(time5),min(time6))
ymax=max(max(time1),max(time2),max(time3),max(time4),max(time5),max(time6))
plt.xlim(xmin,xmax)
plt.ylim(ymin,ymax)

# Adding a grid
major_ticks = ax.get_xticks()
minor_ticks = np.array( [ major_ticks[i]+(major_ticks[i+1]-major_ticks[i])//2 for i in range(len(major_ticks)-1)] )
ax.set_xticks(major_ticks)
#ax.set_xticks(minor_ticks, minor=True)

major_ticks = ax.get_yticks()
#minor_ticks = np.array( [ major_ticks[i]+(major_ticks[i+1]-major_ticks[i])//2 for i in range(len(major_ticks)-1)] )
ax.set_yticks(major_ticks)
#ax.set_yticks(minor_ticks, minor=True)

ax.grid(which='both')

#https://stackoverflow.com/questions/24943991/change-grid-interval-and-specify-tick-labels-in-matplotlib
#https://matplotlib.org/3.5.0/api/_as_gen/matplotlib.pyplot.grid.html

ax.grid(which='major', alpha=0.7, color='grey', linestyle=':', linewidth=1)
ax.grid(which='minor', alpha=0.9, color='grey', linestyle=':', linewidth=0.6)
#ax.grid(which='major', color='grey', linestyle=':', linewidth=1, alpha=0.5)

#OLD
#plt.grid(True, color='grey', ls=':', lw=1)

# Plotting both the curves simultaneously
plt.plot(X, y, 'rs-', linewidth=1, markersize=3.5, label='A&D')
plt.plot(X, z, 'go-', linewidth=1, markersize=4, label='Cunningham') #https://www.geeksforgeeks.org/graph-plotting-in-python-set-1/
plt.plot(X, w, 'b^-', linewidth=1, markersize=4, label='CLSSW-Indep')
plt.plot(X, o, 'y*-', linewidth=1, markersize=5, label='CLSSW-Rank')
#(x, y, color='green', linestyle='dashed', linewidth = 3, marker='o', markerfacecolor='blue', markersize=12)

plt.plot(X, q, 'mx-', linewidth=1, markersize=4, label='EK')
plt.plot(X, p, 'c+-', linewidth=1, markersize=4, label='HK')

# Naming the x-axis, y-axis and the whole graph
plt.xlabel("$n+m$",fontsize=14)
plt.ylabel("Time (microseconds)",fontsize=14)

plt.ticklabel_format(axis="both", style="sci", scilimits=(0,0))

#Adding text at coordinate 8x,y)
#plt.text(60, .025, r'$\mu=100,\ \sigma=15$')

# Adding legend, which helps us recognize the curve according to it's color
plt.legend()

#plt.title('Comparison based on total number of queries')

plt.savefig('results/all_specific_blocks_2.pdf')