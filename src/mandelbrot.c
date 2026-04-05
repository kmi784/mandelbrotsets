#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "../include/mandelbrot.h"

/********** mandelbrot helper **********/

const double LOG2 = 0.6931471805599453;

/**
 * Helper object to handle complex numbers.
 */
typedef struct {
    double real;
    double imag;
} Complex;

/**
 * adds two complex numbers `a` and `b`.
 */
static Complex add(Complex a, Complex b){
    Complex tmp;
    tmp.real = a.real + b.real;
    tmp.imag = a.imag + b.imag;
    return tmp;
}

/**
 * squares a complex number `a`.
 */
static Complex square(Complex a){
    Complex tmp;
    tmp.real = a.real*a.real - a.imag*a.imag;
    tmp.imag = 2*a.real*a.imag;
    return tmp;
}

/**
 * determines complex number corresponding to row and column indices of the grid in 
 * `MandelbrotGrid`.
 * @param grid 
 *      pointer to `MandelbrotGrid` object
 * @param row
 *      row index
 * @param col
 *      column index
 * @return 
 *      corresponding complex number as `Complex` object
 */
static Complex get_complex_number(MandelbrotGrid* grid, unsigned int row, unsigned int col) {
    Complex tmp;
    tmp.real = (
        grid->real_min + (grid->real_max - grid->real_min) * col / (grid->width - 1)
    );
    tmp.imag = (
        grid->imag_min + (grid->imag_max - grid->imag_min) * row / (grid->height - 1)
    );
    return tmp;
}

/**
 * Helper object to store thread-related data.
 */
typedef struct {
    unsigned int row_start;
    unsigned int row_end;
    unsigned int iterations;
    MandelbrotGrid* grid;
} MandelbrotGridStrip;

/**
 * computes the pixels within the passed strip. Intended to be used as worker-function
 * for a thread.
 * @param arg
 * @param grid 
 *      pointer to `GridStrip` object  
 * @return `NULL`
 */
static void* compute_strip(void* arg) {
    MandelbrotGridStrip* strip = (MandelbrotGridStrip*) arg;

    for(unsigned int i = strip->row_start; i < strip->row_end; ++i) {
        for(unsigned int j = 0; j < strip->grid->width; ++j) {
            int bounded = 1;
            Complex z = {0.0, 0.0};
            Complex c = get_complex_number(strip->grid, i, j);

            for(unsigned int k = 0; k < strip->iterations; ++k) {
                z = add(square(z), c);

                if(z.real * z.real + z.imag * z.imag > 4) {
                    strip->grid->pixels[i * strip->grid->width + j] = (
                        k + 2.0 - log(log(z.real * z.real + z.imag * z.imag)) / LOG2
                    );
                    bounded = 0;
                    break;
                }
            }

            if(bounded) {
                strip->grid->pixels[i * strip->grid->width + j] = -1.0;
            }
        }
    }

    return NULL;
}


/********** mandelbrot framework **********/

int init_grid(
    MandelbrotGrid* grid, 
    GridSize size, 
    double real_min, 
    double imag_min, 
    double real_max, 
    double imag_max
) {
    // select heigth of grid
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
                "ERROR: Unkown grid size. Choose between the enums GRID_DEBUG, "
                "GRID_SMALL, GRID_MEDIUM or GRID_LARGE.\n"
            );
            return 1;


    } 

    // compute width of grid
    grid->width = (unsigned int)((double)grid->height 
        * (real_max-real_min)/(imag_max-imag_min));

    // set domains in complex plain
    grid->real_min = real_min;
    grid->real_max = real_max;
    grid->imag_min = imag_min;
    grid->imag_max = imag_max;

    // create pixel array
    grid->pixels = malloc(grid->height * grid->width * sizeof(double));
    if(grid->pixels == NULL) {
        fprintf(stderr, "ERROR: Malloc failed in MandelbrotGrid initiating.\n");
        return 1;
    }

    return 0;
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

int compute_mandelbrot(
    MandelbrotGrid* grid, unsigned int num_iterations, unsigned int num_threads
) {
    pthread_t* threads = malloc(num_threads * sizeof(pthread_t));
    MandelbrotGridStrip* strips = malloc(num_threads * sizeof(MandelbrotGridStrip));
    if(!threads || !strips) {
        fprintf(stderr, "ERROR: Malloc failed in threads initiating.\n");
        return 1;
    }

    unsigned int chunk = (grid->height + num_threads - 1) / num_threads;
    for(unsigned int t = 0; t < num_threads; ++t) {
        strips[t].row_start = t*chunk;
        strips[t].row_end = (t < num_threads - 1) ? (t + 1)*chunk : grid->height;
        strips[t].iterations = num_iterations;
        strips[t].grid = grid;
        pthread_create(&threads[t], NULL, compute_strip, &strips[t]);
    }

    for(unsigned int t = 0; t < num_threads; ++t) {
        pthread_join(threads[t], NULL);
    }

    free(threads);
    free(strips);

    return 0;
}

void free_grid(MandelbrotGrid* grid) {
    if(!grid) return;

    free(grid->pixels);
    grid->pixels = NULL;
}

