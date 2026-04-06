#ifndef RENDER_H
#define RENDER_H

#include <stdio.h>
#include "mandelbrot.h"

/**
 * Enumeration of all possible color maps.
 */
typedef enum {
    COLORMAP_RED
} ColorMap;

/**
 * Image object storing pixel colors and meta data of the image.
 */
typedef struct {
    unsigned int height;
    unsigned int width;
    unsigned char* pixels;
} MandelbrotImage;


/**
 * initiates `MandelbrotImage` object.
 * @param image 
 *      pointer to `MandelbrotImage` object
 * @param height
 *      height of image
 * @param width 
 *      width of image
 * @return `0` on success, `1` on failure
 */
int init_image(MandelbrotImage*, unsigned int height, unsigned int width);

/**
 * renders image data an stores them in `MandelbrotImage` object.
 * @note after calling this function free `MandelbrotGrid` object to
 * avoid huge memory impacts with `free_grid(MandelbrotGrid*)`
 * @param image 
 *      pointer to `MandelbrotImage` object
 * @param grid 
 *      pointer to `MandelbrotGrid` object
 * @param num_iterations
 *      maximal number of iteration per pixel (normalization constant)
 * @param cmap 
 *      identifier for colormap (enums: `COLORMAP_RED`|...)
 * @return `0` on success, `1` on failure
 */
int render_mandelbrot(
    MandelbrotImage* image, 
    const MandelbrotGrid* grid, 
    unsigned int num_iterations, 
    ColorMap cmap
);

/**
 * frees `MandelbrotGrid`-object.
 * @param grid 
 *      pointer to `MandelbrotGrid` object
 */
void free_image(MandelbrotImage* image);

#endif 