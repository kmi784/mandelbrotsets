# Mandelbrot sets

> **Definition (Mandelbrot set)** Let $(z_n)_{n\in\mathbb{N}_0}\subset\mathbb{C}$ be a sequence defined by the recursive formula $$z_{n+1}=z_n^2 + c$$ for $z_0=0$ and $c\in\mathbb{C}$, then the Mandelbrot set is defined by $$M=\left\{c\in\mathbb{C}\  \bigg| \ \sup_{n\in\mathbb{N}_0}|z_n|<\infty\right\}.$$ 

>**Proposition** If there exists a $k\in\mathbb{N}_0$ such that $|z_k|>2$ holds, then $(z_n)_{n\in\mathbb{N}_0}\subset\mathbb{C}$ is unbounded.


## Smooth Coloring

> **Proposition (Asymptotic growth)** Let $(z_n)_{n\in\mathbb{N}_0}\subset\mathbb{C}$ with  $$z_{n+1} = z_n^2 + c.$$  If $|z_n|$ is sufficiently large, then  $$|z_{n+1}| \approx |z_n|^2.$$

> **Lemma (Logarithmic growth)** For sufficiently large $n$ it holds approximately:$$\log |z_{n+1}| \approx 2 \log |z_n|.$$ Iterating this yields:$$\log |z_n| \approx 2^n \cdot \log |z_0|.$$

> **Lemma (Double logarithm linearization)** Applying the logarithm again:$$\log(\log |z_n|) \approx n \cdot \log(2) + \log(\log |z_0|).$$

> **Definition (Continuous escape index)** Let $k\in\mathbb{N}$ be the first index such that $|z_k|>2$. Then define the continuous escape value$$\mu := k + 1 - \frac{\log(\log |z_k|)}{\log(2)}.$$

---

### Interpretation  
The value $\mu \in \mathbb{R}$ refines the discrete escape index $k$ by incorporating the magnitude of $z_k$.  

In particular:
- $k$ gives the coarse escape time,
- the correction term
  $$
  - \frac{\log(\log |z_k|)}{\log(2)}
  $$
  provides a continuous interpolation between iterations.




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