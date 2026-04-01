#include <stdio.h>
#include "../include/mandelbrot.h"

int main() {

    MandelbrotGrid grid;
    if(init_grid(&grid, GRID_MEDIUM, -3.5, -1.5, 1.5, 1.5)) {
        fprintf(stderr, "ERROR: Initiating MandelbrotGrid failed.\n");
        return 1;
    }

    debug_print_grid(&grid);

    if(compute_mandelbrot(&grid, 500, 8)) {
        fprintf(stderr, "ERROR: Computing MandelbrotGrid failed.\n");
        return 1;
    }

    if(save_grid(&grid, "results/grids/medium.txt")) {
        fprintf(stderr, "ERROR: Saving MandelbrotGrid failed.\n");
        return 1;
    }


    free_grid(&grid);    

    return 0;
}