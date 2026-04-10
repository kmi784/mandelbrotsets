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

# define 'compute_rows()' argument types and return type
LIB.compute_rows.argtypes = [POINTER(CMandelbrotGrid), c_uint, c_uint, c_uint]
LIB.compute_rows.restype = None

# define 'free_grid()' argument types and return type
LIB.free_grid.argtypes = [POINTER(CMandelbrotGrid)]
LIB.free_grid.restype = None


def ccompute_strip(
    size: CGridSize,
    real_min: float,
    imag_min: float,
    real_max: float,
    imag_max: float,
    num_iter: int,
    row_start: int,
    row_end: int,
) -> ndarray:
    """
    Compute a horizontal strip of a Mandelbrot grid using the C backend.

    This function initializes a full grid via the C API, computes only the
    specified row range, and returns a NumPy copy of that slice.

    Parameters
    ----------
    size : CGridSize
        Grid dimensions (height, width) passed to the C layer.
    real_min : float
        Minimum value of the real axis.
    imag_min : float
        Minimum value of the imaginary axis.
    real_max : float
        Maximum value of the real axis.
    imag_max : float
        Maximum value of the imaginary axis.
    num_iter : int
        Maximum number of iterations per point.
    row_start : int
        Start index of the row slice (inclusive).
    row_end : int
        End index of the row slice (exclusive).

    Returns
    -------
    ndarray
        2D NumPy array of shape (row_end - row_start, width) containing
        the computed Mandelbrot values.

    Raises
    ------
    RuntimeError
        If initialization of the C grid fails.

    Notes
    -----
    The underlying C memory is freed before returning. The result is a
    copied NumPy array and is independent of the C allocation.
    """
    grid = CMandelbrotGrid()

    rc = LIB.init_grid(
        byref(grid), size.value, real_min, imag_min, real_max, imag_max
    )
    if rc != 0:
        LIB.free_grid(byref(grid))
        raise RuntimeError("running init_grid() failed")

    LIB.compute_rows(byref(grid), num_iter, row_start, row_end)

    # copy array before free memory (malloc) 
    strip = as_array(grid.pixels, shape=(grid.height * grid.width,)).reshape(
        (grid.height, grid.width)
    )[row_start:row_end].copy()

    LIB.free_grid(byref(grid))

    return row_start, row_end, strip
