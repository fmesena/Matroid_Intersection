import numpy as np
from matplotlib import pyplot as plt

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

plt.tight_layout()
plt.savefig('foo2.png')