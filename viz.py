import matplotlib.pyplot as plt
import matplotlib.animation as animation

PATH = "./n-body.dat"
PADDING = 10

def get_extreme(extreme, coord, data):	
	return extreme([sample[coord] for sample in data])

def get_extremes(data):
	max_extreme = (
		get_extreme(max, "x", data) + PADDING, 
		get_extreme(max, "y", data) + PADDING, 
		get_extreme(max, "z", data) + PADDING
	)
	
	min_extreme = (
		get_extreme(min, "x", data) - PADDING, 
		get_extreme(min, "y", data) - PADDING, 
		get_extreme(min, "z", data) - PADDING
	)

	count = get_extreme(max, "id", data)+1

	return count, max_extreme, min_extreme

def read_data():
	data = []

	with open(PATH, "r") as f:
		lines = f.readlines()
		for line in lines:
			split = line.split(' ')
			sample = dict()
			sample["time"] = float(split[0])
			sample["id"] = int(split[1])
			sample["x"] = float(split[2])
			sample["y"] = float(split[3])
			sample["z"] = float(split[4])
			data.append(sample)

	return data

data = read_data()
count, max_ex, min_ex = get_extremes(data)

filtered_data = []
for i in range(count):
	filtered_data.append([sample for sample in data if sample["id"] == i])

fig, ax = plt.subplots(1, 1)

def animate(i):
	ax.clear()

	for j in range(count):		
		ax.plot(filtered_data[j][i]["x"], filtered_data[j][i]["y"], color='green', label='original', marker='o')

	ax.set_xlim([min_ex[0], max_ex[0]])
	ax.set_ylim([min_ex[1], max_ex[1]])


ani = animation.FuncAnimation(fig, animate, frames=int(len(data)/count), interval=50, repeat=False)

# To save the animation using Pillow as a gif
writer = animation.PillowWriter(fps=15,
                                metadata=dict(artist='Me'),
                                bitrate=1800)
ani.save('n-body.gif', writer=writer)

plt.show()