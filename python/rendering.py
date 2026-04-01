import matplotlib.pyplot as plt
from matplotlib.colors import LinearSegmentedColormap
from numpy import log, loadtxt

key = "Sasuke"

scale = (
    # "small"
    # "medium"
    "large"
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


with open(f"data/array_{scale}.txt", "r") as f:
    header = f.readline()
    height, width, max_iter = map(int, header.split())


print(f"Reading array of shape {height}x{width}...")

image = loadtxt(f"data/array_{scale}.txt", skiprows=1)
assert image.shape == (height, width)

print(f"Reading succeeded.")

new_height = int(10 / 16 * width)
crop = (height - new_height) // 2
image = image[crop : crop + new_height, :]


mask_inside = image == max_iter

image = log(1 + image)
image[mask_inside] = 0

cmap = LinearSegmentedColormap.from_list("my_cmap", color_maps[key])

#plt.imshow(image, cmap=cmap)
#plt.colorbar()
#plt.show()

plt.imsave(f"images/Mandelbrot Set {key}.png", image, cmap=cmap)
