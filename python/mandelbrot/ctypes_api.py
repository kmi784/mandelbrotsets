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
    _fields_ = [
        ("pixels", POINTER(c_double)),
        ("height", c_uint),
        ("width", c_uint),
        ("real_min", c_double),
        ("real_max", c_double),
        ("imag_min", c_double),
        ("imag_max", c_double),
    ]

# define init_grid() parameter types and return types
LIB.init_grid.argtypes = [
    POINTER(CMandelbrotGrid),
    c_int,
    c_double,
    c_double,
    c_double,
    c_double,
]
LIB.init_grid.restype = c_int

# define compute_rows() parameter types and return type
LIB.compute_rows.argtypes = [POINTER(CMandelbrotGrid), c_uint, c_uint, c_uint]
LIB.compute_rows.restype = None

# define free_grid() parameter types and return type
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
    """ctype-api"""
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

    return strip
