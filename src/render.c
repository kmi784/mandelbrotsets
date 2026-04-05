#include <math.h>
#include "../include/render.h"

typedef struct {
    unsigned int r;
    unsigned int g;
    unsigned int b;
} RGB;

static RGB gray_scale(double t) {
    RGB color;
    color.r = (unsigned int)(255.0 * t);
    color.g = (unsigned int)(255.0 * t);
    color.b = (unsigned int)(255.0 * t);
    return color;
}

static RGB blue_red_scale(double t) {
    RGB color;
    color.r = (unsigned int)(255.0 * t);
    color.g = 0;
    color.b = (unsigned int)(255.0 * (1.0 - t));
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

    double norm = log(1.0 + num_iterations);
    for(unsigned int i = 0; i < grid->height; ++i) {
        for(unsigned int j = 0; j < grid->width; ++j) {
            unsigned int pixel_value = grid->pixels[i * grid->width + j];
            if(pixel_value == num_iterations) {
                fprintf(file, "0 0 0\t");
            } else {
                RGB color = blue_red_scale(log(1.0 + pixel_value) / norm);
                fprintf(file, "%u %u %u\t", color.r, color.g, color.b);
            }
        }
        fprintf(file, "\n");
    }
    

    return 0;
}