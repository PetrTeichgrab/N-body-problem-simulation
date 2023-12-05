import pandas as pd
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from matplotlib.animation import FuncAnimation

# Načtení dat z CSV souboru
file_path = 'n-body.dat'
data = pd.read_csv(file_path, dtype={'id': int, 'time': float, 'x': float, 'y': float, 'z': float})

# Předpokládáme, že data obsahují sloupce 'id', 'time', 'x', 'y', 'z'
# Případně upravte názvy sloupců podle skutečného obsahu souboru
id_column = 'id'
time_column = 'time'
x_column = 'x'
y_column = 'y'
z_column = 'z'

# Získání unikátních identifikátorů planet
planet_ids = data[id_column].unique()

fig, ax = plt.subplots(1, 1)

def animate(i):
	ax.clear()
	# Get the point from the points list at index i
	# print(data["id"][i])
	# Plot that point using the x and y coordinates
	ax.plot(data["x"][i], data["y"][i], color='green', label='original', marker='o')
	# # Set the x and y axis to display a fixed range
	ax.set_xlim([1.9, 2.09])
	ax.set_ylim([1.5, 2.5])

ani = FuncAnimation(fig, animate, frames=100, interval=100, repeat=False)
plt.show()