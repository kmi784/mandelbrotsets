from enum import Enum
from multiprocessing import Process, shared_memory

import matplotlib.pyplot as plt
from numpy import zeros, linspace, log, ndarray

LOG2 = log(2)


class GridSize(Enum):
    debug = 350
    small = 4000
    medium = 8000
    large = 12000


class MandelbrotSet:
    """
    
    """
    def __init__(
        self,
        size: GridSize,
        real_min: float,
        imag_min: float,
        real_max: float,
        imag_max: float,
    ):
        self._shape = (
            int(size.value),
            int(size.value * (real_max - real_min) / (imag_max - imag_min)),
        )
        self._real_range = linspace(real_min, real_max, self._shape[1], dtype=float)
        self._imag_range = linspace(imag_min, imag_max, self._shape[0], dtype=float)

        self._shm = shared_memory.SharedMemory(
            create=True, size=zeros(self._shape, dtype=float).nbytes
        )
        self._grid = ndarray(shape=self._shape, dtype=float, buffer=self._shm.buf)
        self._grid[:] = 0

    def cleanup(self) -> None:
        self._shm.close()
        self._shm.unlink()

    def compute_with_py(self, num_iter: int, num_processes: int = 1) -> None:
        """
        computes multi-processed numeric pixel values 

        Parameters
        ----------
        `num_iter`: `int`
            maximal number of iteration per pixel
        `num_processes`: `int`
            number of processes 
        """
        chunk = (self._shape[0] + num_processes - 1) // num_processes
        processes: list[Process] = []
        for p in range(num_processes):
            row_start = p * chunk 
            row_end = min((p + 1) * chunk, self._shape[0])
            if row_start < row_end:
                processes.append(
                    Process(
                        target=self._compute_strip,
                        args=(self._shm.name, num_iter, row_start, row_end),
                    )
                )

        for process in processes:
            process.start()

        for process in processes:
            process.join()


    def compute_with_c(self, num_iter: int, num_threads: int):
        pass

    def draw(self):
        """visualizes Mandelbrotsets with 'inferno' color map"""
        plt.imshow(self._grid, cmap="inferno")
        plt.axis("off")
        plt.show()

    def _compute_strip(self, name: str, num_iter: int, row_start: int, row_end: int):
        """helper to compute numeric pixel values of a strip of 'self._grid'"""
        shm = shared_memory.SharedMemory(name=name)
        grid = ndarray(shape=self._shape, dtype=float, buffer=shm.buf)

        for i in range(row_start, row_end):
            for j in range(self._shape[1]):
                z = 0j
                c = self._real_range[j] + 1j * self._imag_range[i]
                for k in range(num_iter):
                    z = z * z + c
                    if z.real**2 + z.imag**2 > 4:
                        grid[i, j] = (
                            k + 2.0 - log(log(z.real**2 + z.imag**2)) / LOG2
                        )
                        break
                else:
                    grid[i, j] = -1
        shm.close()


if __name__ == "__main__":

    obj = MandelbrotSet(GridSize.small, -3.4, -1.5, 1.4, 1.5)
    obj.compute_with_py(num_iter=100, num_processes=8)
    obj.draw()
    obj.cleanup()
