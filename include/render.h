#ifndef RENDER_H
#define RENDER_H

#include <stdio.h>
#include "mandelbrot.h"

/**
 * renders `MandelbrotGrid` object to ppm
 * @param grid 
 *      pointer to `MandelbrotGrid` object
 * @param num_iterations
 *      maximal number of iteration per pixel
 * @param file
 *      pointer to `FILE` object
 * @return `0` on success, `1` on failure
 */
int render_ppm(const MandelbrotGrid* grid, unsigned int num_iterations, FILE* file);

#endif 