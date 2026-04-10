from ctypes import CDLL, c_double, c_int, c_uint, Structure, byref, POINTER
from enum import Enum
from pathlib import Path

from numpy import ndarray
from numpy.ctypeslib import as_array

LIB = CDLL(Path(__file__).parent / "../../lib/libmandelbrot.so")


class CGridSize(Enum):
    debug = 0
    small = 1
    medium = 2
    large = 3


class CMandelbrotGrid(Structure):
    """
    Python representation of the C struct `MandelbrotGrid` using ctypes.

    Fields
    ------
    pixels : POINTER(c_double)
        Pointer to a contiguous array of doubles storing the grid values
        (typically row-major order).
    height : c_uint
        Number of rows in the grid.
    width : c_uint
        Number of columns in the grid.
    real_min : c_double
        Minimum value of the real axis.
    real_max : c_double
        Maximum value of the real axis.
    imag_min : c_double
        Minimum value of the imaginary axis.
    imag_max : c_double
        Maximum value of the imaginary axis.
    """

    # define memory layout of the struct 'MandelbrotGrid'
    _fields_ = [
        ("pixels", POINTER(c_double)),
        ("height", c_uint),
        ("width", c_uint),
        ("real_min", c_double),
        ("real_max", c_double),
        ("imag_min", c_double),
        ("imag_max", c_double),
    ]


# define 'init_grid()' argument types and return types
LIB.init_grid.argtypes = [
    POINTER(CMandelbrotGrid),
    c_int,
    c_double,
    c_double,
    c_double,
    c_double,
]
LIB.init_grid.restype = c_int

# define 'compute_mandelbrot()' argument types and return types
LIB.compute_mandelbrot.argtypes = [POINTER(CMandelbrotGrid), c_uint, c_uint]
LIB.compute_mandelbrot.restype = c_int

# define 'free_grid()' argument types and return type
LIB.free_grid.argtypes = [POINTER(CMandelbrotGrid)]
LIB.free_grid.restype = None


def compute_mandelbrot(
    size: CGridSize,
    real_min: float,
    imag_min: float,
    real_max: float,
    imag_max: float,
    num_iter: int,
    num_threads: int,
) -> ndarray:
    """
    Compute a Mandelbrot grid using the C backend.

    Parameters
    ----------
    `size` : `CGridSize`
        Grid dimensions (height, width) passed to the C layer.
    `real_min` : `float`
        Minimum value of the real axis.
    `imag_min` : `float`
        Minimum value of the imaginary axis.
    `real_max` : `float`
        Maximum value of the real axis.
    `imag_max` : `float`
        Maximum value of the imaginary axis.
    `num_iter` : `int`
        Maximum number of iterations per point.
    `num_threads` : `int`
        number of threads.

    Returns
    -------
    `ndarray`
        2D NumPy array containing the computed Mandelbrot values.

    Raises
    ------
    RuntimeError
        If initialization or compuation of the C grid fails.

    Notes
    -----
    The underlying C memory is freed before returning. The result is a
    copied NumPy array and is independent of the C allocation.
    """
    grid = CMandelbrotGrid()

    rc = LIB.init_grid(byref(grid), size.value, real_min, imag_min, real_max, imag_max)
    if rc != 0:
        LIB.free_grid(byref(grid))
        raise RuntimeError("running 'init_grid()' failed")

    rc = LIB.compute_mandelbrot(byref(grid), num_iter, num_threads)
    if rc != 0:
        LIB.free_grid(byref(grid))
        raise RuntimeError("running 'compute_mandelbrot()' failed")

    # copy array before free memory (malloc)
    out = as_array(grid.pixels, shape=(grid.height * grid.width,)).reshape(
        (grid.height, grid.width)
    ).copy()

    LIB.free_grid(byref(grid))
    return out
