#include <math.h>
#include <stdlib.h>
#include "../include/render.h"

/********** render helper **********/

/**
 * Helper object to handle rgb-colors
 */
typedef struct {
    unsigned int r;
    unsigned int g;
    unsigned int b;
} RGB;

static const RGB RGB_BLACK = {0, 0, 0}; // #000000
static const RGB RGB_WHITE = {255, 255, 255}; // #ffffff


/**
 * helper checking `RGB` represents actually a color
 * @param color
 * @return `1` is a rgb-color, `0` is not a rgb-color
 */
static int check_rgb(RGB color) {
    if(color.r > 255 || color.g > 255 || color.b > 255) {
        return 0;
    }
    return 1;
}

/**
 * helper to assign pixel in `MandelbrotImage` according rgb-color
 * @param image
 *      pointer to `MandelbrotImage` object
 * @param row
 *      row index of grid
 * @param col
 *      column index of grid
 * @param color
 *      color which should be assigned
 * @return `0` success, `1` failure
 */
static int assign_image_pixel(
    MandelbrotImage* image, 
    unsigned int row, 
    unsigned int col, 
    const RGB color
) {
    if(!check_rgb(color)) {
        fprintf(
            stderr, "ERROR: pixel-assignment failed, color (%u, %u, %u) is not a rgb color.\n", color.r, color.g, color.b
        );
        return 1;
    }
    unsigned int idx = row * image->width + col;
    image->pixels[idx * 3 + 0] = (unsigned char)color.r; 
    image->pixels[idx * 3 + 1] = (unsigned char)color.g; 
    image->pixels[idx * 3 + 2] = (unsigned char)color.b; 
    return 0;
}

/**
 * interpolates linearly two colors
 * @param u 
 *      run parameter between `0.0` and `1,0`
 * @param color_start 
 *      start node interpolating range (at `u=0.0`)
 * @param color_end
 *      end node interpolating range (at `u=1.0`)
 * @return `RGB` color at `u`
 */
static RGB linear_interpolation(double u, const RGB color_start, const RGB color_end) {
    RGB color;
    color.r = (unsigned int)((double)color_start.r + u * ((double)color_end.r - (double)color_start.r));
    color.g = (unsigned int)((double)color_start.g + u * ((double)color_end.g - (double)color_start.g));
    color.b = (unsigned int)((double)color_start.b + u * ((double)color_end.b - (double)color_start.b));

    return color;
} 

/**
 * color map 
 * v black 
 * | #1d4678 (29 70 120) 
 * v #3584e4 (53 132 228)
 * | white
 * @param t 
 *      run parameter
 * @return `RGB` object of corresponding color 
 */
static RGB blue_colormap(double t) {
    RGB color;
    RGB c0 = RGB_BLACK;
    RGB c1 = {29, 70, 120};   
    RGB c2 = {53, 132, 228};
    RGB c3 = RGB_WHITE;

    if(t < 0.4) {
        color = linear_interpolation(t / 0.4, c0, c1);
    } else if(t < 0.5) {
        color = linear_interpolation((t - 0.4) / 0.1, c1, c2);
    } else {
        color = linear_interpolation((t - 0.5) / 0.5, c2, c3);
    }
    return color;
}

/**
 * color map 
 * v black 
 * | white
 * @param t 
 *      run parameter
 * @return `RGB` object of corresponding color 
 */
static RGB gray_colormap(double t) {
    return linear_interpolation(t, RGB_BLACK, RGB_WHITE);
}

/**
 * color map 
 * v black 
 * | #1a4c23 (26 76 35) 
 * v #3a944a (58 148 74)
 * | white
 * @param t 
 *      run parameter
 * @return `RGB` object of corresponding color 
 */
static RGB green_colormap(double t) {
    RGB color;
    RGB c0 = RGB_BLACK;
    RGB c1 = {26, 76, 35};   
    RGB c2 = {58, 148, 74};  
    RGB c3 = RGB_WHITE;

    if(t < 0.4) {
        color = linear_interpolation(t / 0.4, c0, c1);
    } else if(t < 0.5) {
        color = linear_interpolation((t - 0.4) / 0.1, c1, c2);
    } else {
        color = linear_interpolation((t - 0.5) / 0.5, c2, c3);
    }
    return color;
}

 /**
 * color map 
 * v black 
 * | #482055 (72 32 85) 
 * v #9141ac (145 65 172)
 * | white
 * @param t 
 *      run parameter
 * @return `RGB` object of corresponding color 
 */
static RGB purple_colormap(double t) {
    RGB color;
    RGB c0 = RGB_BLACK;
    RGB c1 = {72, 32, 85};    
    RGB c2 = {145, 65, 172};  
    RGB c3 = RGB_WHITE;

    if(t < 0.4) {
        color = linear_interpolation(t / 0.4, c0, c1);
    } else if(t < 0.5) {
        color = linear_interpolation((t - 0.4) / 0.1, c1, c2);
    } else {
        color = linear_interpolation((t - 0.5) / 0.5, c2, c3);
    }
    return color;
}

/**
 * color map 
 * v black 
 * | #691820 (105 24 32) 
 * v #e62d42 (230 45 66)
 * | white
 * @param t 
 *      run parameter
 * @return `RGB` object of corresponding color 
 */
static RGB red_colormap(double t) {
    RGB color;
    RGB color0 = RGB_BLACK;
    RGB color1 = {105, 24, 32};
    RGB color2 = {230, 45, 66};
    RGB color3 = RGB_WHITE;
    if(t < 0.4) {
        color = linear_interpolation(t / 0.4, color0, color1);
    } else if( 0.4 <= t && t < 0.5) {
        color = linear_interpolation((t - 0.4) / 0.1, color1, color2);
    } else if(0.5<= t) {
        color =  linear_interpolation((t - 0.5) / 0.5, color2, color3);
    }
    return color;
}

/**
 * color map 
 * v black 
 * | #765100 (118 81 0) 
 * v #c88800 (200 136 0)
 * | white
 * @param t 
 *      run parameter
 * @return `RGB` object of corresponding color 
 */
static RGB yellow_colormap(double t) {
    RGB color;
    RGB c0 = RGB_BLACK;
    RGB c1 = {118, 81, 0};   
    RGB c2 = {200, 136, 0};  
    RGB c3 = RGB_WHITE;

    if(t < 0.4) {
        color = linear_interpolation(t / 0.4, c0, c1);
    } else if(t < 0.5) {
        color = linear_interpolation((t - 0.4) / 0.1, c1, c2);
    } else {
        color = linear_interpolation((t - 0.5) / 0.5, c2, c3);
    }
    return color;
}

/**
 * color map 
 * v black 
 * | #5a0000 (90 0 0) 
 * v #fc6739 (252 103 57)
 * | #c88800 (200 136 0)
 * @param t 
 *      run parameter
 * @return `RGB` object of corresponding color 
 */
static RGB darksouls_colormap(double t) {
    RGB color;
    RGB c0 = RGB_BLACK;
    RGB c1 = {90, 0, 0};     
    RGB c2 = {252, 103, 57}; 
    RGB c3 = {200, 136, 0};  

    if(t < 0.3) {
        color = linear_interpolation(t / 0.3, c0, c1);
    } else if(t < 0.5) {
        color = linear_interpolation((t - 0.3) / 0.2, c1, c2);
    } else {
        color = linear_interpolation((t - 0.5) / 0.5, c2, c3);
    }
    return color;
}

/**
 * color map 
 * v black 
 * | midnightblue (25 25 112) 
 * v indigo (75 0 130)
 * | purple (128 0 128)
 * @param t 
 *      run parameter
 * @return `RGB` object of corresponding color 
 */
static RGB sasuke_colormap(double t) {
    RGB color;
    RGB c0 = RGB_BLACK;
    RGB c1 = {25, 25, 112};   
    RGB c2 = {75, 0, 130};    
    RGB c3 = {128, 0, 128};  

    if(t < 0.4) {
        color = linear_interpolation(t / 0.4, c0, c1);
    } else if(t < 0.5) {
        color = linear_interpolation((t - 0.4) / 0.1, c1, c2);
    } else {
        color = linear_interpolation((t - 0.5) / 0.5, c2, c3);
    }
    return color;
}

/**
 * get rgb-color according passed `ColorMap` idendidfire 
 * @param cmap
 *      identifier for colormap (enums: `COLORMAP_RED`|...)
 * @param pixel_value
 *      numeric pixel value
 * @param num_iterations
 *      maximal number of iteration per pixel (normalization constant)
 */
static RGB colormap(ColorMap cmap, double pixel_value, unsigned int num_iterations) {
    double t;
    switch(cmap) {
        case COLORMAP_BLUE:
            t = fmod(pixel_value * 0.02, 1.0);
            return blue_colormap(t);

        case COLORMAP_GRAY:
            t = pow(pixel_value / num_iterations, 0.5);
            return gray_colormap(t);
        
        case COLORMAP_GREEN:
            t = fmod(pixel_value * 0.02, 1.0);
            return green_colormap(t);

        case COLORMAP_PURPLE:
            t = fmod(pixel_value * 0.02, 1.0);
            return purple_colormap(t);

        case COLORMAP_RED:
            t = fmod(pixel_value * 0.02, 1.0);
            return red_colormap(t);

        case COLORMAP_YELLOW:
            t = fmod(pixel_value * 0.02, 1.0);
            return yellow_colormap(t);

        case COLORMAP_DARKSOULS:
            t = fmod(pixel_value * 0.02, 1.0);
            return darksouls_colormap(t);

        case COLORMAP_SASUKE:
            t = fmod(pixel_value * 0.02, 1.0);
            return sasuke_colormap(t);

        default:
            return RGB_BLACK;   
    }
}

/********** render api **********/

int init_image(MandelbrotImage* image, unsigned int height, unsigned int width) {
    // set meta data
    image->height = height;
    image->width = width;

    // creat pixel array
    image->pixels = malloc(3 * (height * width) * sizeof(unsigned char));
    if(image->pixels == NULL) {
        fprintf(stderr, "ERROR: Malloc failed in MandelbrotImage initiating.\n");
        return 1;
    }

    return 0;
}

int render_mandelbrot(
    MandelbrotImage* image, 
    const MandelbrotGrid* grid, 
    unsigned int num_iterations, 
    ColorMap cmap
) {
    if(image->height != grid->height || image->width != grid->width) {
        fprintf(stderr, "ERROR: shape of image and grid objects are different.\n");
        return 1;
    }

    
    for(unsigned int i = 0; i < image->height; ++i) {
        for(unsigned int j = 0; j < image->width; ++j) {
            double pixel_value = grid->pixels[i * grid->width + j];
            if(pixel_value < 0.0) {
                if(assign_image_pixel(image, i, j, RGB_BLACK)) {return 1;}
            } else {
                if(
                    assign_image_pixel(
                        image, i, j, colormap(cmap, pixel_value, num_iterations)
                    )
                ) {return 1;}
            }
        }
    }

    return 0;
}

void free_image(MandelbrotImage* image) {
    if(!image) return;
    free(image->pixels);
    image->pixels = NULL;
}
