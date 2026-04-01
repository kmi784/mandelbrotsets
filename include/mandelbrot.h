#ifndef MANDELBROT_H
#define MANDELBROT_H


typedef enum {
    GRID_DEBUG,
    GRID_SMALL,
    GRID_MEDIUM,
    GRID_LARGE,
} GridSize;


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
 * frees `MandelbrotGrid`-object.
 * @param grid 
 *      pointer to `MandelbrotGrid` object
 */
void free_grid(MandelbrotGrid* grid);

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
 * @param iterations
 *      maximal number of iteration per pixel
 * @param threads
 *      number of threads 
 */
int compute_mandelbrot(MandelbrotGrid* grid, unsigned int iterations, unsigned int threads);

#endif