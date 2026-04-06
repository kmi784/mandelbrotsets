#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "../src/render.c"

static void test_init_image(void) {
    MandelbrotImage image;
    assert(init_image(&image, 100, 200) == 0);
    assert(image.height == 100);
    assert(image.width == 200);
    assert(image.pixels != NULL);

    free_image(&image);
}

static void test_free_image_set_null(void) {
    MandelbrotImage image;
    assert(init_image(&image, 100, 200) == 0);
    assert(image.pixels != NULL);
    free_image(&image);
    assert(image.pixels == NULL);
}

static void test_render_image_different_shapes_of_image_and_grid(void) {
    MandelbrotGrid grid;
    assert(init_grid(&grid, GRID_DEBUG, -2, -1, 1, 1) == 0);
    MandelbrotImage image;
    assert(init_image(&image, 100, 200) == 0);

    assert(render_mandelbrot(&image, &grid, 10, COLORMAP_RED) != 0);

    free_grid(&grid);
    free_image(&image);
}

static void test_render_mandelbrot_black_for_bounded_pixel(void) {
    MandelbrotGrid grid;
    assert(init_grid(&grid, GRID_DEBUG, -2.0, -1.0, 1.0, 1.0) == 0);

    MandelbrotImage image;
    assert(init_image(&image, grid.height, grid.width) == 0);

    /* bounded pixel */
    unsigned int row = (grid.height - 1) / 2;
    unsigned int col = 2 * (grid.width - 1) / 3;
    for(unsigned int i = 0; i < grid.height * grid.width; ++i) {
        grid.pixels[i] = 1.0;
    }
    grid.pixels[row * grid.width + col] = -1.0;

    assert(render_mandelbrot(&image, &grid, 50, COLORMAP_RED) == 0);

    unsigned int idx = (row * image.width + col) * 3;
    assert(image.pixels[idx + 0] == 0);
    assert(image.pixels[idx + 1] == 0);
    assert(image.pixels[idx + 2] == 0);

    free_grid(&grid);
    free_image(&image);
}

static void test_render_mandelbrot_gray_assigns_equal_channels(void) {
    MandelbrotGrid grid;
    assert(init_grid(&grid, GRID_DEBUG, -2.0, -1.0, 1.0, 1.0) == 0);

    MandelbrotImage image;
    assert(init_image(&image, grid.height, grid.width) == 0);

    for(unsigned int i = 0; i < grid.height * grid.width; ++i) {
        grid.pixels[i] = 25.0;
    }

    assert(render_mandelbrot(&image, &grid, 100, COLORMAP_GRAY) == 0);

    unsigned int idx = 0;
    assert(image.pixels[idx + 0] == image.pixels[idx + 1]);
    assert(image.pixels[idx + 1] == image.pixels[idx + 2]);

    free_grid(&grid);
    free_image(&image);
}

static void test_render_mandelbrot_non_bounded_pixel_not_black(void) {
    MandelbrotGrid grid;
    assert(init_grid(&grid, GRID_DEBUG, -2.0, -1.0, 1.0, 1.0) == 0);

    MandelbrotImage image;
    assert(init_image(&image, grid.height, grid.width) == 0);

    for(unsigned int i = 0; i < grid.height * grid.width; ++i) {
        grid.pixels[i] = 10.0;
    }

    assert(render_mandelbrot(&image, &grid, 100, COLORMAP_RED) == 0);

    unsigned int idx = 0;
    assert(
        image.pixels[idx + 0] != 0 ||
        image.pixels[idx + 1] != 0 ||
        image.pixels[idx + 2] != 0
    );

    free_grid(&grid);
    free_image(&image);
}

static void test_render_mandelbrot_gray_white_at_max_iterations(void) {
    MandelbrotGrid grid;
    assert(init_grid(&grid, GRID_DEBUG, -2.0, -1.0, 1.0, 1.0) == 0);

    MandelbrotImage image;
    assert(init_image(&image, grid.height, grid.width) == 0);

    for(unsigned int i = 0; i < grid.height * grid.width; ++i) {
        grid.pixels[i] = 100.0;
    }

    assert(render_mandelbrot(&image, &grid, 100, COLORMAP_GRAY) == 0);

    unsigned int idx = 0;
    assert(image.pixels[idx + 0] == 255);
    assert(image.pixels[idx + 1] == 255);
    assert(image.pixels[idx + 2] == 255);

    free_grid(&grid);
    free_image(&image);
}

int main() {
    printf(">>>>>> Starting tests 'test_render.c'...\n");
    printf("------ api-tests\n");
    test_init_image();
    test_free_image_set_null();
    test_render_image_different_shapes_of_image_and_grid();

    printf("------ render-tests\n");
    test_render_mandelbrot_black_for_bounded_pixel();
    test_render_mandelbrot_gray_assigns_equal_channels();
    test_render_mandelbrot_non_bounded_pixel_not_black();
    test_render_mandelbrot_gray_white_at_max_iterations();

    printf("<<<<<< All tests in 'test_render.c' passed.\n");
    return 0;
}