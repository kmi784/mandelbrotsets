#include <stdio.h>
#include <stdlib.h>
#include "../include/mandelbrot.h"

int main(int argc, char* argv[]){
    int rc = 1;

    if(argc != 2) {
        fprintf(
            stderr, 
            "ERROR: You have to pass exactly 1 argument (number of threads) to '%s'\n", 
            argv[0]
        );
        return rc;
    }

    double real_min = -3.4;
    double real_max = 1.4;
    double imag_min = -1.5;
    double imag_max = 1.5;

    unsigned int num_iter = 500;
    unsigned int num_threads = (unsigned int)strtoul(argv[1], NULL, 10);

    printf("Starting calculation with %u threads...\n", num_threads);

    MandelbrotGrid grid;
    if(init_grid(&grid, GRID_MEDIUM, real_min, imag_min, real_max, imag_max)) {
        fprintf(stderr, "ERROR: Initiating MandelbrotImage failed.\n");
        goto cleanup;
    }

    if(compute_mandelbrot(&grid, num_iter, num_threads)) {
        fprintf(stderr, "ERROR: Computing MandelbrotGrid failed.\n");
        goto cleanup;
    }

    printf("Finished calculation.\n");

    rc = 0;

    cleanup:
        free_grid(&grid);

    return rc;
}