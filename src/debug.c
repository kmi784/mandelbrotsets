#include <stdio.h>
#include "../include/cli.h"
#include "../include/mandelbrot.h"
#include "../include/render.h"
#include "../include/export.h"

int main(){
    MandelbrotGrid grid;
    init_grid(&grid, GRID_DEBUG, -3.5, -1.5, 1.5, 1.5);
    debug_print_grid(&grid);
    compute_mandelbrot(&grid, 500, 8);

    FILE* file = fopen("results/figures/debug.ppm", "w");
    if(file == NULL) {
        fprintf(stderr, "ERROR");
        return 1;
    }
    render_ppm(&grid, 500, file);
    
    free_grid(&grid);
    fclose(file);

    return 0;
}