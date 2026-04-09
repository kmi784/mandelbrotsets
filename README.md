# Overview
This project has two main goals:
1) Provide a CLI application for generating Mandelbrotset wallpapers, like
    <p align="center">
      <img src="results/example.png" width="500"/>
    </p>
2) Compare the performance of numeric pixel computation using:
   - C (`pthreads`)
   - Python (`multiprocessing`)
   - a hybrid approach (`multiprocessing` + `ctypes`)

# Quick start
~~~sh
git clone https://github.com/kmi784/mandelbrotsets.git
cd mandelbrotsets
make release
bin/mandelbrot --output image
~~~


# Features
- Mandelbrotset computation 
- RGB rendering with different colormaps
- Multithreading in C (`pthreads`)
- Multiprocessing in Python
- Hybrid C/Python execution via ctypes
- Basic benchmarking


## Build
> **Dependencies:**
> - Python >=3.10
> - (GCC) >= 10
> - GNU Make >= 4

### C (CLI application)
To compile the CLI-application enter
~~~sh
make release
~~~

### C (benchmarking)
To compile the benchmark binaries and the backend shared-lib enter
~~~sh
make benchmark
~~~

### C (testing)
To compile and run the unit-tests enter
~~~sh
make tests
~~~

### C (debugging)
To compile and run debug entry enter
~~~sh
make debug # default rule
~~~

### Python 
To install the python requirements
~~~sh
pip install -r python/requirements.txt
~~~

## Run

### Running CLI-application
To generate an image enter
~~~sh
bin/mandelbrot --output image --resolution 4k --cmap red --num_threads 4
~~~
To generate raw data enter
~~~sh
bin/mandelbrot --output data --real_min -3.5 --real_max 1.5
~~~
For more information about the possible options enter
~~~sh
bin/mandelbrot --help
~~~


### Running benchmarks
Make sure you have 
- installed the python requirements `pip install -r python/requirements.txt`
- compiled the benchmark binaries and shred-lin `make benchmark`
Then you can run the [notebook](python/benchmarks.ipynb).

## Project structure

~~~sh
mandelbrotsets/
│── bin/
│── build/
│── include/
│   │── cli.h # frontend
│   │── export.h # export service
│   │── mandelbrot.h # backend
│   │── render.h # image rendering
│   ╰── stb_image_write.h # writing png
│── lib/
│   ╰── libmandelbrot.so # backend for (c + python)
│── python/
│   │── mandelbrot/
│   │   │── __init__.py 
│   │   │── core.py # image rendering + backend
│   │   ╰── ctypes_api.py # helper for (c + python)
│   │── benchmarks.ipynb # running/visualize benchmarks
│   │── cpy_benchmark.py # entrypoint benchmark (c + python)
│   │── py_benchmark.py # entrypoint benchmark (python)
│   │── requirements.txt # dependencies (python)
│   ╰─ utils.py # helper executing benchmark binary/entrypoint
│── results/
│   ╰─ example.png
├── src/
│   │── cli.c # frontend
│   │── debug.c # entrypoint debugging
│   │── export.c # export service
│   │── main.c # entrypoint for CLI-application
│   │── mandelbrot.c # backend
│   ╰── render.c # image rendering
│── tests/
│   ╰── test_*.c/ # unit-tests
│── Makefile
╰── README.md
~~~

## Notes

---

*Definition (Mandelbrotset):* Let $(z_n)_{n\in\mathbb{N}_0}\subset\mathbb{C}$ be a sequence defined by the recursive formula $$z_{n+1}=z_n^2 + c$$ for $z_0=0$ and $c\in\mathbb{C}$, then the Mandelbrot set is defined by $$M=\left\{c\in\mathbb{C}\  \bigg| \ \sup_{n\in\mathbb{N}_0}|z_n|<\infty\right\}.$$ 

---

*Proposition:* If there exists a $k\in\mathbb{N}_0$ such that $|z_k|>2$ holds, then $(z_n)_{n\in\mathbb{N}_0}\subset\mathbb{C}$ is unbounded.


### Smooth Coloring

---

*Remark:* Let $(z_n)_{n\in\mathbb{N}_0}\subset\mathbb{C}$ with  $$z_{n+1} = z_n^2 + c.$$  If $|z_n|$ is sufficiently large, then  $$|z_{n+1}| \approx |z_n|^2.$$

---

*Remark:* For sufficiently large $n$ it holds approximately:$$\log |z_{n+1}| \approx 2 \log |z_n|.$$ Iterating this yields:$$\log |z_n| \approx 2^n \cdot \log |z_0|.$$

---

*Remark:* Applying the logarithm again:$$\log(\log |z_n|) \approx n \cdot \log(2) + \log(\log |z_0|).$$

---

*Definition (Continuous escape index):* Let $k\in\mathbb{N}$ be the first index such that $|z_k|>2$. Then define the continuous escape value$$\mu := k + 1 - \frac{\log(\log |z_k|)}{\log(2)}.$$


