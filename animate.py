import csv
import numpy as np
from matplotlib import pyplot as plt
from matplotlib import animation

fig = plt.figure()
ax = plt.axes(xlim=(0, 10), ylim=(-15, 15))

fig_mass = plt.figure()
ax_mass = plt.axes(xlim=(-10, 10), ylim=(-10, 10))

mass_1 = plt.Rectangle((0, 0), 1, 1, linewidth=1, edgecolor='r', facecolor='none')
mass_2 = plt.Rectangle((0, 0), 3, 3, linewidth=1, edgecolor='b', facecolor='none')

lines = []
for i in range(6):
	line, = ax.plot([], [], lw=2)
	lines.insert(i, line)

data = []

csv.register_dialect('skip_space', skipinitialspace=True)
with open('mpc.data', 'r') as f:
	reader=csv.reader(f , delimiter=' ', dialect='skip_space')
	for item in reader:
		data.append(item)

def init():
	line.set_data([], [])
	ax_mass.add_patch(mass_1)
	ax_mass.add_patch(mass_2)
	return line,

def animate(i):
	global force_arrow
	global net_force_arrow

	x = np.linspace(0, 10, 1000)
	y_data = [0,0,0,0,0,0]
	for y_idx in range(6):
		y_data[y_idx] = np.linspace(0, 0, 1000)

	for y_idx in range(6):
		for idx in range(i):
			y_data[y_idx][idx] = data[idx][y_idx]

	for y_idx in range(6):
		lines[y_idx].set_data(x, y_data[y_idx])


	mass_1.xy = (float(data[i][2])-0.5, 0)
	mass_2.xy = (float(data[i][5])-1.5, -3)

	if i > 0:
		ax_mass.patches.remove(force_arrow)
		ax_mass.patches.remove(net_force_arrow)

	force_arrow = plt.Arrow(float(data[i][2]), 1, float(data[i][1]) * 10, 0, color = 'r')
	net_force_arrow = plt.Arrow(float(data[i][2]), 2, float(data[i][4]) * 10, 0, color = 'g')
	ax_mass.add_patch(force_arrow)
	ax_mass.add_patch(net_force_arrow)


	fig_mass.canvas.draw()

	return lines[1], lines[2], lines[3], lines[4], lines[5],

anim = animation.FuncAnimation(fig, animate, init_func=init, frames=1000, interval=1, blit=True, repeat=False)

plt.show()