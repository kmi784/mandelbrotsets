#ifndef EXPORT_H
#define EXPORT_H

#include "mandelbrot.h"
#include "render.h"
#include <stdio.h>

/**
 * export numeric grid values to a `txt`-file.
 * @param grid
 *      pointer to `MandelbrotGrid` object
 * @param file
 *      pointer to `FILE` object
 * @return `0` on success, `1` on failure
 */
int save_grid_txt(const MandelbrotGrid *grid, FILE *file);

/**
 * export image to a `ppm`-file.
 * @param image
 *      pointer to `MandelbrotImage` object
 * @param file
 *      pointer to `FILE` object
 * @return `0` on success, `1` on failure
 */
int save_image_ppm(const MandelbrotImage *image, FILE *file);

/**
 * export image to a `png`-file.
 * @param image
 *      pointer to `MandelbrotImage` object
 * @param path
 *      pointer to path "string"
 * @return `0` on success, `1` on failure
 */
int save_image_png(const MandelbrotImage *image, const char *path);

#endif
