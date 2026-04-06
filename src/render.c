#include <math.h>
#include <stdlib.h>
#include "../include/render.h"

/********** render helper **********/


/**
 * Helper object to handle rgb-colors
 */
typedef struct {
    unsigned int r;
    unsigned int g;
    unsigned int b;
} RGB;

static const RGB RGB_BLACK = {0, 0, 0}; // #000000
static const RGB RGB_WHITE = {255, 255, 255}; // #ffffff


/**
 * helper checking `RGB` represents actually a color
 * @param color
 * @return `1` is a rgb-color, `0` is not a rgb-color
 */
static int check_rgb(RGB color) {
    if(color.r > 255 || color.g > 255 || color.b > 255) {
        return 0;
    }
    return 1;
}

/**
 * helper to assign pixel in `MandelbrotImage` according rgb-color
 * @param image
 *      pointer to `MandelbrotImage` object
 * @param row
 *      row index of grid
 * @param col
 *      column index of grid
 * @param color
 *      color which should be assigned
 * @return `0` success, `1` failure
 */
static int assign_image_pixel(
    MandelbrotImage* image, 
    unsigned int row, 
    unsigned int col, 
    const RGB color
) {
    if(!check_rgb(color)) {
        fprintf(
            stderr, "ERROR: pixel-assignment failed, color ist not an rgb color.\n"
        );
        return 1;
    }
    unsigned int idx = row * image->width + col;
    image->pixels[idx * 3 + 0] = (unsigned char)color.r; 
    image->pixels[idx * 3 + 1] = (unsigned char)color.b; 
    image->pixels[idx * 3 + 2] = (unsigned char)color.b; 
    return 0;
}

/**
 * interpolates linearly two colors
 * @param u 
 *      run parameter between `0.0` and `1,0`
 * @param color_start 
 *      start node interpolating range (at `u=0.0`)
 * @param color_end
 *      end node interpolating range (at `u=1.0`)
 * @return `RGB` color at `u`
 */
static RGB linear_interpolation(double u, const RGB color_start, const RGB color_end) {
    RGB color;
    color.r = (unsigned int)(color_start.r + u * (color_end.r - color_start.r));
    color.g = (unsigned int)(color_start.g + u * (color_end.g - color_start.g));
    color.b = (unsigned int)(color_start.b + u * (color_end.b - color_start.b));

    return color;
} 

/**
 * color map 
 * v black 
 * | #691820 (105 24 32) 
 * v #e62d42 (230 45 66)
 * | white
 * @param t 
 *      run parameter
 * @return `RGB` object of corresponding color 
 */
static RGB red_colormap(double t) {
    RGB color;
    RGB color0 = RGB_BLACK;
    RGB color1 = {105, 24, 32};
    RGB color2 = {230, 45, 66};
    RGB color3 = RGB_WHITE;
    if(t < 0.4) {
        color = linear_interpolation(t / 0.4, color0, color1);
    } else if( 0.4 <= t && t < 0.5) {
        color = linear_interpolation((t - 0.4) / 0.1, color1, color2);
    } else if(0.5<= t) {
        color =  linear_interpolation((t - 0.5) / 0.5, color2, color3);
    }
    return color;
}

/********** render api **********/

int init_image(MandelbrotImage* image, unsigned int height, unsigned int width) {
    // set meta data
    image->height = height;
    image->width = width;

    // creat pixel array
    image->pixels = malloc(3 * (height * width) * sizeof(unsigned char));
    if(image->pixels == NULL) {
        fprintf(stderr, "ERROR: Malloc failed in MandelbrotImage initiating.\n");
        return 1;
    }

    return 0;
}

int render_mandelbrot(
    MandelbrotImage* image, 
    const MandelbrotGrid* grid, 
    unsigned int num_iterations, 
    ColorMap cmap
) {
    if(image->height != grid->height || image->width != grid->width) {
        fprintf(stderr, "ERROR: shape of image and grid objects are different.\n");
        return 1;
    }

    
    for(unsigned int i = 0; i < image->height; ++i) {
        for(unsigned int j = 0; j < image->width; ++j) {
            double pixel_value = grid->pixels[i * grid->width + j];
            if(pixel_value < 0.0) {
                if(assign_image_pixel(image, i, j, RGB_BLACK)) {return 1;}
            } else {
                // double t = pow(pixel_value / num_iterations, 0.5);
                double t = fmod(pixel_value * 0.02, 1.0);
                if(assign_image_pixel(image, i, j, red_colormap(t))) {return 1;}
            }
        }
    }

    return 0;
}

void free_image(MandelbrotImage* image) {
    if(!image) return;
    free(image->pixels);
    image->pixels = NULL;
}
