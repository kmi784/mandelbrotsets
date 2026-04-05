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

static RGB red_colormap(double t) {
    /**
     * t0 = #000000 <-> 0 0 0
     * t1 = #691820 <-> 105 24 32
     * t2 = #e62d42 <-> 230 45 66
     * t3 = #ffffff <-> 255 255 255
     */
    RGB color;
    if(t < 0.4) {
        double u = t/0.4;
        color.r = (unsigned int)(0   + u * (105 - 0));
        color.g = (unsigned int)(0   + u * ( 24 - 0));
        color.b = (unsigned int)(0   + u * ( 32 - 0));
    } else if( 0.4 <= t && t < 0.5) {
        double u = (t - 0.4) / 0.1;
        color.r = (unsigned int)(105 + u * (230 - 105));
        color.g = (unsigned int)(24 + u * (45 - 24));
        color.b = (unsigned int)(32 + u * (66 - 32));
    } else if(0.5<= t) {
        double u = (t - 0.5) / 0.5;
        color.r = (unsigned int)(230 + u * (255 - 230));
        color.g = (unsigned int)(45 + u * (255 - 45));
        color.b = (unsigned int)(66 + u * (255 - 66));
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