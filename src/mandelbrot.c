#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "../include/mandelbrot.h"

int init_grid(
    MandelbrotGrid* grid, 
    GridSize size, 
    double real_min, 
    double imag_min, 
    double real_max, 
    double imag_max
) {
    // select heigth of image
    switch(size) {
        case GRID_DEBUG:
            grid->height = 350;
            break;
        case GRID_SMALL:
            grid->height = 4000;
            break;
        case GRID_MEDIUM:
            grid->height = 8000;
            break;
        case GRID_LARGE:
            grid->height = 12000;
            break;
        default:
            fprintf(
                stderr, 
                "ERROR: Unkown image size. Choose between the enums IMAGE_DEBUG, "
                "IMAGE_SMALL, IMAGE_MEDIUM or IMAGE_LARGE.\n"
            );
            return 1;


    } 

    // compute width of image
    grid->width = (unsigned int)((double)grid->height 
        * (real_max-real_min)/(imag_max-imag_min));

    // set domains in complex plain
    grid->real_min = real_min;
    grid->real_max = real_max;
    grid->imag_min = imag_min;
    grid->imag_max = imag_max;

    // create pixel array
    grid->pixels = malloc(grid->height * grid->width * sizeof(unsigned int));
    if(grid->pixels == NULL) {
        fprintf(stderr, "ERROR: Malloc failed in Image initiating.\n");
        return 1;
    }

    return 0;
}

void free_grid(MandelbrotGrid* grid) {
    if(!grid) return;

    free(grid->pixels);
    grid->pixels = NULL;
}

void debug_print_grid(const MandelbrotGrid* grid) {
    printf(
        "shape: %ux%u, complex set: (%f, %f)x(%f, %f)\n", 
        grid->height, 
        grid->width, 
        grid->real_min, 
        grid->real_max, 
        grid->imag_min, 
        grid->imag_max
    );
}