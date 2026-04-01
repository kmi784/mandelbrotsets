class MandelbrotGrid:
    pass 

if __name__ == "__main__":
    import matplotlib.pyplot as plt 
    from numpy import linspace, array, zeros, savetxt

    number_pixels = 10
    number_iterations = 200

    image = zeros((number_pixels, number_pixels), dtype=int)

    for i, real in enumerate(linspace(-2.5, 1, number_pixels)):
        for j, imag in enumerate(linspace(-1.5, 1.5, number_pixels)):
            z = 0
            c = real + 1j * imag
            for k in range(number_iterations):
                z = z**2 + c
                if z.real**2+z.imag**2 > 4:
                    image[j, i] = k
                    break
            else: 
                image[j, i] = number_iterations

    savetxt("check.txt", image, fmt="%i")


    plt.imshow(image, cmap="inferno")
    plt.axis("equal")
    plt.show()
