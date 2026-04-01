#include <stdio.h>
#include "../include/mandelbrot.h"

int main() {

    MandelbrotGrid grid;
    if(init_grid(&grid, GRID_LARGE, -3.0, -1.5, 1.5, 1.5)) {
        fprintf(stderr, "ERROR: Initiating Image failed.\n");
        return 1;
    }

    debug_print_grid(&grid);


    free_grid(&grid);    

    return 0;
}