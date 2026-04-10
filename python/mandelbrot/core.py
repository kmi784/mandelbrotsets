from enum import Enum
from multiprocessing import Process, shared_memory, Pool

import matplotlib.pyplot as plt
from numpy import ndarray, linspace, log, zeros

from .ctypes_api import CGridSize, ccompute_strip

LOG2 = log(2)


class GridSize(Enum):
    debug = 350
    small = 2000
    medium = 4000
    large = 8000


CMAP = {
    GridSize.debug: CGridSize.debug,
    GridSize.small: CGridSize.small,
    GridSize.medium: CGridSize.medium,
    GridSize.large: CGridSize.large,
}


class MandelbrotSet:
    """
    Represent a discretized region of the complex plane for Mandelbrot set
    computation and visualization.

    The class builds a 2D sampling grid from the given real and imaginary
    bounds, stores the computed escape-time values in shared memory, and
    provides utilities to compute and display the result.

    Parameters
    ----------
    `size` : `GridSize`
        Preset controlling the grid height.
    `real_min` : `float`
        Lower bound of the real axis.
    `imag_min` : `float`
        Lower bound of the imaginary axis.
    `real_max` : `float`
        Upper bound of the real axis.
    `imag_max` : `float`
        Upper bound of the imaginary axis.

    Notes
    -----
    - The grid height is determined by the selected `GridSize`.
    - The grid width is derived from the aspect ratio of the complex-plane
      window defined by the input bounds.
    - Computed pixel values are stored as floating-point escape values.
    - Points that do not escape within the iteration limit are stored as `-1`.
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

        self._csize = CMAP[size]

    def cleanup(self) -> None:
        """
        Release the shared-memory resources used by the Mandelbrot grid.

        This closes the local shared-memory handle and unlinks the underlying
        shared-memory block. It should be called once the grid is no longer needed.
        """
        self._shm.close()
        self._shm.unlink()

    def compute_with_py(self, num_iter: int, num_processes: int = 1) -> None:
        """
        Compute Mandelbrot escape values using Python multiprocessing.

        The grid is divided into horizontal strips, and each strip is processed
        by a separate worker process writing into the shared-memory array.

        Parameters
        ----------
        `num_iter` : `int`
            Maximum number of iterations used to test whether a point escapes.
        `num_processes` : `int`, `default=1`
            Number of worker processes.

        Notes
        -----
        Each pixel stores a smoothed escape-time value. Points that do not escape
        within `num_iter` iterations are marked with `-1`.
        """
        chunk = (self._shape[0] + num_processes - 1) // num_processes
        processes: list[Process] = []
        for p in range(num_processes):
            row_start = p * chunk
            row_end = min((p + 1) * chunk, self._shape[0])
            if row_start < row_end:
                processes.append(
                    Process(
                        target=MandelbrotSet._compute_strip,
                        args=(
                            self._shm.name,
                            self._shape,
                            self._real_range,
                            self._imag_range,
                            num_iter,
                            row_start,
                            row_end,
                        ),
                    )
                )

        for process in processes:
            process.start()

        for process in processes:
            process.join()

    def compute_with_c(self, num_iter: int, num_processes: int):
        """
        Compute Mandelbrot escape values using the C backend.

        Parameters
        ----------
        `num_iter` : `int`
            Maximum number of iterations used to test whether a point escapes.
        `num_threads` : `int`
            Number of native threads used by the C implementation.
        """
        chunk = (self._shape[0] + num_processes - 1) // num_processes
        args = []
        for p in range(num_processes):
            row_start = p * chunk
            row_end = min((p + 1) * chunk, self._shape[0])
            if row_start < row_end:
                args.append(
                    (
                        self._csize,
                        self._real_range[0],
                        self._imag_range[0],
                        self._real_range[-1],
                        self._imag_range[-1],
                        num_iter,
                        row_start,
                        row_end
                    )
                )

        # since worker returning strip there is no shared memory routine needed
        with Pool(num_processes) as pool:
            # starting and joining process using 'starmap' instead of 'map', 
            # because worker has more then one parameter 
            for row_start, row_end, strip in pool.starmap(ccompute_strip, args):
                self._grid[row_start: row_end] = strip


    def draw(self):
        """
        Display the computed Mandelbrot grid as an image.

        The numeric grid is rendered with Matplotlib using the `"inferno"`
        colormap and with axes hidden.
        """
        plt.imshow(self._grid, cmap="inferno")
        plt.axis("off")
        plt.show()

    @staticmethod
    def _compute_strip(
        name: str,
        shape: tuple[int, int],
        real_range: ndarray,
        imag_range: ndarray,
        num_iter: int,
        row_start: int,
        row_end: int,
    ):
        """
        Compute Mandelbrot escape values for a contiguous strip of rows.

        This worker method attaches to an existing shared-memory block and fills
        the rows in the half-open interval `[row_start, row_end)`.

        Parameters
        ----------
        `name` : `str`
            Name of the shared-memory block containing the output grid.
        `num_iter` : `int`
            Maximum number of iterations used to test whether a point escapes.
        `row_start` : `int`
            First row index to compute (inclusive).
        `row_end` : `int`
            Last row index to compute (exclusive).

        Notes
        -----
        Escaping points receive a smoothed escape-time value based on the final
        magnitude of `z`. Points that do not escape are assigned `-1`.
        """
        shm = shared_memory.SharedMemory(name=name)
        grid = ndarray(shape=shape, dtype=float, buffer=shm.buf)

        for i in range(row_start, row_end):
            for j in range(shape[1]):
                z = 0j
                c = real_range[j] + 1j * imag_range[i]
                for k in range(num_iter):
                    z = z * z + c
                    if z.real**2 + z.imag**2 > 4:
                        grid[i, j] = k + 2.0 - log(log(z.real**2 + z.imag**2)) / LOG2
                        break
                else:
                    grid[i, j] = -1
        shm.close()


if __name__ == "__main__":

    obj = MandelbrotSet(GridSize.large, -3.4, -1.5, 1.4, 1.5)
    obj.compute_with_c(num_iter=200, num_processes=8)
    # obj.compute_with_py(num_iter=100, num_processes=8)
    obj.draw()
    obj.cleanup()
