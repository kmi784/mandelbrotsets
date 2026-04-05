#include <stdio.h>

#include "../include/export.h"

int save_grid(const MandelbrotGrid* grid, FILE* file) {
   if(file == NULL) {
        fprintf(stderr, "ERROR: Opening file failed.\n");
        return 1;
    }

    for(unsigned int i = 0; i < grid->height; ++i){
        for(unsigned int j = 0; j < grid->width; ++j){
            fprintf(file, "%u\t", grid->pixels[i * grid->width +  j]);
        }
        fprintf(file, "\n");
    }

    return 0;
}

