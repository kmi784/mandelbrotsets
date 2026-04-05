#ifndef EXPORT_H
#define EXPORT_H

#include <stdio.h>
#include "mandelbrot.h"

/**
 * saves grid to a file.
 * @param grid 
 *      pointer to `MandelbrotGrid` object
 * @param file
 *      pointer to `FILE` object
 * @return `0` on success, `1` on failure
 */
int save_grid(const MandelbrotGrid* grid, FILE* file);


#endif