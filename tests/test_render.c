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

int main() {
    printf(">>>>>> Starting tests 'test_render.c'...\n");
    printf("------ api-tests\n");
    test_init_image();
    test_free_image_set_null();
    test_render_image_different_shapes_of_image_and_grid();

    printf("<<<<<< All tests in 'test_render.c' passed.\n");
    return 0;
}