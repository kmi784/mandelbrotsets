import matplotlib.pyplot as plt
from matplotlib.colors import LinearSegmentedColormap
from numpy import log, loadtxt

key = "Sasuke"

scale = (
    # "small"
    "medium"
    # "large"
)


color_maps = {
    "Blue": [(0.0, "black"), (0.4, "#1d4678"), (0.5, "#3584e4"), (1.0, "white")],
    "Green":  [(0.0, "black"), (0.4, "#1a4c23"), (0.5, "#3a944a"), (1.0, "white")],
    "Purple": [(0.0, "black"), (0.4, "#482055"), (0.5, "#9141ac"), (1.0, "white")],
    "Red": [(0.0, "black"), (0.4, "#691820"), (0.5, "#e62d42"), (1.0, "white")],    
    "Yellow":  [(0.0, "black"), (0.4, "#765100"), (0.5, "#c88800"), (1.0, "white")],
    "Dark Souls": [(0.0, "black"), (0.3, "#5a0000"), (0.5, "#fc6739"), (1.0, "#c88800")],
    "Sasuke": [(0.0, "black"), (0.4, "midnightblue"), (0.5, "indigo"), (1.0, "purple")],
}


grid = loadtxt(f"results/grids/{scale}.txt")
height, width = grid.shape

print(f"Loading grid of shape {height}x{width} succeeded.")

#new_height = int(10 / 16 * width)
#crop = (height - new_height) // 2
#grid = grid[crop : crop + new_height, :]


mask_inside = grid == grid.max()

grid = log(1 + grid)
grid[mask_inside] = 0

cmap = LinearSegmentedColormap.from_list("my_cmap", color_maps[key])

plt.imshow(grid, cmap=cmap)
plt.colorbar()
plt.show()

#plt.imsave(f"results/figures/Mandelbrot Set {key}.png", grid, cmap=cmap)
