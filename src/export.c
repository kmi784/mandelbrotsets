#include <stdio.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../include/stb_image_write.h"
#include "../include/export.h"

int save_grid_txt(const MandelbrotGrid* grid, FILE* file) {
   if(file == NULL) {
        fprintf(stderr, "ERROR: Opening file failed.\n");
        return 1;
    }

    for(unsigned int i = 0; i < grid->height; ++i){
        for(unsigned int j = 0; j < grid->width; ++j){
            fprintf(file, "%f\t", grid->pixels[i * grid->width +  j]);
        }
        fprintf(file, "\n");
    }

    return 0;
}

int save_image_ppm(const MandelbrotImage* image, FILE* file) {
   if(file == NULL) {
        fprintf(stderr, "ERROR: Opening file failed.\n");
        return 1;
    }

    fprintf(file, "P6\n%u %u\n255\n", image->width, image->height);
    fwrite(image->pixels, sizeof(unsigned char), image->width * image->height * 3, file);
    return 0;
}

int save_image_png(const MandelbrotImage* image, const char* path) {
   if(path == NULL) {
        fprintf(stderr, "ERROR: invalid path.\n");
        return 1;
    }

    int ok = stbi_write_png(
        path, 
        image->width,
        image->height,
        3, 
        image->pixels,
        image->width * 3
    );

    if(!ok) {
        fprintf(stderr, "ERROR: Writing png-file failed.\n");
        return 1;
    }
    return 0;
}

