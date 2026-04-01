# Mandelbrot sets

> **Definition (Mandelbrot set)** Let $(z_n)_{n\in\mathbb{N}_0}\subset\mathbb{C}$ be a sequence defined by the recursive formula $$z_{n+1}=z_n^2 + c$$ for $z_0=0$ and $c\in\mathbb{C}$, then the Mandelbrot set is defined by $$M=\left\{c\in\mathbb{C}\  \bigg| \ \sup_{n\in\mathbb{N}_0}|z_n|<\infty\right\}.$$ 

>**Proposition** If there exists a $k\in\mathbb{N}_0$ such that $|z_k|>2$ holds, then $(z_n)_{n\in\mathbb{N}_0}\subset\mathbb{C}$ is unbounded.



~~~sh
repo/
|-- Makefile
|-- README.md
|-- bin/
|-- build/
|-- include/
|   |-- mandelbrot.h # interface MandelbrotGrid-struct
|-- src/
|   |-- mandelbrot.c # MandelbrotGrid-struct
|   |-- debug.c # entry point for debug
|   |-- main.c # entry point for release
|-- lib/
|   |-- libmandelbrot.a
|-- python/
|   |-- mandelbrot/
|   |   |-- __init__.py 
|   |   |-- core.py # MandelbrotGrid-class
|   |   |-- ctypes_api.py # api ctypes 
|   |-- benchmarks.ipynb # uses function from utils.py for benchmarking
|   |-- rendering.py # uses function from utils.py for rendering
|   |-- utils.py # functions that uses binaries or python objects 
|-- results/
|   |-- grids/ # stores grid as txt
|   |-- figures/ # stores images of mandelbrot sets
~~~