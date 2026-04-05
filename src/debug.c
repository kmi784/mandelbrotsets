#include <stdio.h>
#include "../include/mandelbrot.h"
#include "../include/cli.h"

//int main() {

//    MandelbrotGrid grid;
//    if(init_grid(&grid, GRID_DEBUG, -3.5, -1.5, 1.5, 1.5)) {
//        fprintf(stderr, "ERROR: Initiating MandelbrotGrid failed.\n");
//        return 1;
//    }

//    debug_print_grid(&grid);

//    if(compute_mandelbrot(&grid, 500, 8)) {
//        fprintf(stderr, "ERROR: Computing MandelbrotGrid failed.\n");
//        return 1;
//    }

//    FILE* file = fopen("results/grids/debug.txt", "w");
//    if(file == NULL) {
//        fprintf(stderr, "ERROR: Opening file failed.\n");
//        return 1;
//    }

//    if(save_grid(&grid, file)) {
//        fprintf(stderr, "ERROR: Saving MandelbrotGrid failed.\n");
//        return 1;
//    }

//    fclose(file);

//    free_grid(&grid);    

//    return 0;
//}

int main(int argc, char* argv[]){

    Configs configs;
    set_defaults(&configs);

    int rc = parse_args(&configs, argc, argv);
    if(rc == 1) {return 0;}
    if(rc == -1) {return 1;}

    if(!validate_configs(&configs)) {return 1;}

    if(!finalize_configs(&configs)) {return 1;}

    if (!configs.assume_yes && !confirm_configs(&configs)){return 0;}

    return run_program(&configs);
}