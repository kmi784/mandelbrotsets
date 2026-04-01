#ifndef MANDELBROT_H
#define MANDELBROT_H

/**
 * Enumeration of all possible grid sizes.
 */
typedef enum {
    GRID_DEBUG,
    GRID_SMALL,
    GRID_MEDIUM,
    GRID_LARGE,
} GridSize;

/**
 * Core-object storing the pixels of the grid and its domain in the complex plain.
 */
typedef struct {
    unsigned int* pixels;
    unsigned int height;
    unsigned int width;
    double real_min;
    double real_max;
    double imag_min;
    double imag_max;
} MandelbrotGrid;

/**
 * initiates `MandelbrotGrid` object.
 * @param grid 
 *      pointer to `MandelbrotGrid` object
 * @param size 
 *      identifier for grid size (enums: `GRID_DEBUG`, `GRID_SMALL`, 
 *      `GRID_MEDIUM`, `GRID_LARGE`)
 * @param real_min 
 *      smallest real part of considered complex domain 
 * @param imag_min 
 *      smallest imaginary part of considered complex domain 
 * @param real_max 
 *      larges real part of considered complex domain 
 * @param imag_max 
 *      larges imaginary part of considered complex domain 
 * @return `0` on success, `1` on failure
 */
int init_grid(
    MandelbrotGrid* grid, 
    GridSize size, 
    double real_min, 
    double imag_min, 
    double real_max, 
    double imag_max
);

/**
 * shows stored metadata in `MandelbrotGrid`-object.
 * @param grid 
 *      pointer to `MandelbrotGrid` object
 */
void debug_print_grid(const MandelbrotGrid* grid); 


/**
 * assigns pixels in grid with values.
 * @param grid 
 *      pointer to `MandelbrotGrid` object
 * @param num_iterations
 *      maximal number of iteration per pixel
 * @param num_threads
 *      number of threads 
 * @return `0` on success, `1` on failure
 */
int compute_mandelbrot(
    MandelbrotGrid* grid, unsigned int num_iterations, unsigned int num_threads
);

/**
 * saves grid to a file.
 * @param grid 
 *      pointer to `MandelbrotGrid` object
 * @param file
 *      pointer to `FILE` object
 * @return `0` on success, `1` on failure
 */
int save_grid(MandelbrotGrid* grid, FILE* file);

/**
 * frees `MandelbrotGrid`-object.
 * @param grid 
 *      pointer to `MandelbrotGrid` object
 */
void free_grid(MandelbrotGrid* grid);

#endif