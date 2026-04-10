#!/usr/bin/env python

if __name__ == "__main__":
    import sys
    from mandelbrot import GridSize, MandelbrotSet

    if len(sys.argv) != 2:
        raise ValueError(
            f"You have to pass exactly 1 argument (number of threads) to '{sys.argv[0]}'"
        )

    num_threads = int(sys.argv[1])

    print(f"Starting calculation with {num_threads} processes...")

    obj = MandelbrotSet(
        GridSize.debug, real_min=-0.5, imag_min=-0.5, real_max=0.5, imag_max=0.5
    )
    obj.compute_with_c(2000, num_threads)

    print("Finished calculation.")

    obj.cleanup()