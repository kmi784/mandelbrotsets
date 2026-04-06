#include <math.h>
#include "../include/render.h"

/**
 * Helper object to handle rgb-colors
 */
typedef struct {
    unsigned int r;
    unsigned int g;
    unsigned int b;
} RGB;

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
 * v #000000 (0 0 0) 
 * | #691820 (105 24 32) 
 * v #e62d42 (230 45 66)
 * | #ffffff (255 255 255)
 * @param t 
 *      run parameter
 * @return `RGB` color at `t` 
 */
static RGB red_colormap(double t) {
    RGB color;
    RGB color0 = {0 ,0 ,0};
    RGB color1 = {105, 24, 32};
    RGB color2 = {230, 45, 66};
    RGB color3 = {255, 255, 255};
    if(t < 0.4) {
        color = linear_interpolation(t / 0.4, color0, color1);
    } else if( 0.4 <= t && t < 0.5) {
        color = linear_interpolation((t - 0.4) / 0.1, color1, color2);
    } else if(0.5<= t) {
        color=  linear_interpolation((t - 0.5) / 0.5, color2, color3);
    }
    return color;
}

int render_ppm(const MandelbrotGrid* grid, unsigned int num_iterations, FILE* file) {
   if(file == NULL || grid == NULL || num_iterations == 0) {
        fprintf(stderr, "ERROR: Invalid input for 'render_ppm'.\n");
        return 1;
    }

    fprintf(file, "P3\n");
    fprintf(file, "%u %u\n", grid->width, grid->height);
    fprintf(file, "255\n");

    for(unsigned int i = 0; i < grid->height; ++i) {
        for(unsigned int j = 0; j < grid->width; ++j) {
            double pixel_value = grid->pixels[i * grid->width + j];
            if(pixel_value < 0.0) {
                fprintf(file, "0 0 0\t");
            } else {
                // double t = pow(pixel_value / num_iterations, 0.5);
                double t = fmod(pixel_value * 0.02, 1.0);
                RGB color = red_colormap(t);
                fprintf(file, "%u %u %u\t", color.r, color.g, color.b);
            }
        }
        fprintf(file, "\n");
    }
    

    return 0;
}