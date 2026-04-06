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

    //FILE* file = fopen("results/figures/debug.ppm", "w");
    //if(file == NULL) {
    //    fprintf(stderr, "ERROR");
    //    return 1;
    //}
    //// save_grid_txt(&grid, file);
    //render_ppm(&grid, 500, file);
    
    //free_grid(&grid);
    //fclose(file);

    MandelbrotImage image;
    if(init_image(&image, grid.height, grid.width)) {return 1;}
    if(render_mandelbrot(&image, &grid, 500, COLORMAP_RED)) {return 1;}
    free_grid(&grid);

    FILE* file = fopen("results/figures/debug.ppm", "w");
    if(file == NULL) {
        fprintf(stderr, "ERROR");
        return 1;
    }

    save_image_ppm(&image, file);

    free_image(&image);
    fclose(file);


    return 0;
}