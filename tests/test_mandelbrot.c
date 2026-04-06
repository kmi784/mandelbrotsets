#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "../src/mandelbrot.c"

// API-tests
static void test_init_grid_shape(void) {
    MandelbrotGrid grid;
    assert(init_grid(&grid, GRID_DEBUG, -2.0, -1.0, 1.0, 1.0) == 0);

    assert(grid.height == 350);
    assert(grid.width == 525);
    assert(grid.pixels != NULL);
    assert(grid.real_min == -2.0);
    assert(grid.real_max == 1.0);
    assert(grid.imag_min == -1.0);
    assert(grid.imag_max == 1.0);

    free_grid(&grid);
}

static void test_init_grid_invalid_size(void) {
    MandelbrotGrid grid;
    assert(init_grid(&grid, (GridSize)999, -2.0, -1.0, 1.0, 1.0) != 0);
}

static void test_free_grid_sets_null(void) {
    MandelbrotGrid grid;
    assert(init_grid(&grid, GRID_DEBUG, -2.0, -1.0, 1.0, 1.0) == 0);

    assert(grid.pixels != NULL);
    free_grid(&grid);
    assert(grid.pixels == NULL);
}

static void test_compute_mandelbrot_center_is_bounded(void) {
    MandelbrotGrid grid;
    assert(init_grid(&grid, GRID_DEBUG, -2.0, -1.0, 1.0, 1.0) == 0);
    assert(compute_mandelbrot(&grid, 50, 1) == 0);

    unsigned int row_origin = (grid.height - 1) / 2;
    unsigned int col_origin = 2 * (grid.width - 1) / 3; 
    assert(grid.pixels[row_origin * grid.width + col_origin] == -1);
    
    free_grid(&grid);
}

static void test_compute_mandelbrot_corners_are_unbounded(void) {
    MandelbrotGrid grid;
    assert(init_grid(&grid, GRID_DEBUG, -2.0, -1.0, 1.0, 1.0) == 0);
    assert(compute_mandelbrot(&grid, 50, 1) == 0);

    unsigned int row_end = grid.height - 1;
    unsigned int col_end = grid.width - 1;
    assert(grid.pixels[0 * grid.width + 0] > 0);
    assert(grid.pixels[0 * grid.width + col_end] > 0);
    assert(grid.pixels[row_end * grid.width + col_end] > 0);
    assert(grid.pixels[row_end * grid.width + 0] > 0);

    free_grid(&grid);
}

static void test_complex_add(void) {
    Complex a = {1.5, -2.0};
    Complex b = {-1.0, 2.5};

    Complex result = add(a, b);
    assert(fabs(result.real - 0.5) < 1e-12);
    assert(fabs(result.imag - 0.5) < 1e-12);
}

static void test_complex_square(void) {
    Complex a = {1.0, 2.0};
    Complex result = square(a);

    assert(fabs(result.real + 3.0) < 1e-12);
    assert(fabs(result.imag - 4) < 1e-12);
}

static void test_get_complex_numbers(void) {
    MandelbrotGrid grid;
    assert(init_grid(&grid, GRID_DEBUG, -2.0, -1.0, 1.0, 1.0) == 0);

    unsigned int row_end = grid.height - 1;
    unsigned int col_end = grid.width - 1;

    Complex near_origin = get_complex_number(&grid,  row_end / 2, 2 * col_end / 3);
    assert(fabs(near_origin.real) < 1e-2);
    assert(fabs(near_origin.imag) < 1e-2);

    Complex upper_left = get_complex_number(&grid, 0, 0);
    assert(fabs(grid.real_min - upper_left.real) < 1e-12);
    assert(fabs(grid.imag_min - upper_left.imag) < 1e-12);

    Complex upper_right = get_complex_number(&grid, 0, col_end);
    assert(fabs(grid.real_max - upper_right.real) < 1e-12);
    assert(fabs(grid.imag_min - upper_right.imag) < 1e-12);

    Complex lower_left = get_complex_number(&grid, row_end, 0);
    assert(fabs(grid.real_min - lower_left.real) < 1e-12);
    assert(fabs(grid.imag_max - lower_left.imag) < 1e-12);

    Complex lower_right = get_complex_number(&grid, row_end, col_end);
    assert(fabs(grid.real_max - lower_right.real) < 1e-12);
    assert(fabs(grid.imag_max - lower_right.imag) < 1e-12);

    free_grid(&grid);
}

static void test_compute_strip(void) {
    MandelbrotGrid grid;
    assert(init_grid(&grid, GRID_DEBUG, -2.0, -1.0, 1.0, 1.0) == 0);

    for(unsigned int i = 0; i < grid.height; ++i) {
        for(unsigned int j = 0; j < grid.width; ++j) {
            grid.pixels[i * grid.width + j] = -100;
        }
    }

    MandelbrotGridStrip strip;
    strip.row_start = 0;
    strip.row_end = 5;
    strip.iterations = 5;
    strip.grid = &grid;
    compute_strip(&strip);
    for(unsigned int i = strip.row_start; i < strip.row_end; ++i) {
        for(unsigned int j = 0; j < grid.width; ++j) {
            assert(grid.pixels[i * grid.width + j] > -100.0);
        }
    }

    free_grid(&grid);
}

static void test_compare_thread_results(void) {
    MandelbrotGrid grid1;
    assert(init_grid(&grid1, GRID_DEBUG, -2.0, -1.0, 1.0, 1.0) == 0);
    assert(compute_mandelbrot(&grid1, 30, 1) == 0);

    MandelbrotGrid grid2;
    assert(init_grid(&grid2, GRID_DEBUG, -2.0, -1.0, 1.0, 1.0) == 0);
    assert(compute_mandelbrot(&grid2, 30, 4) == 0);

    assert(grid1.width == grid2.width);
    assert(grid1.height == grid2.height);

    for(unsigned int i = 0; i < grid1.height; ++i) {
        for(unsigned int j = 0; j < grid1.width; ++j) {
            assert(
                fabs(
                    grid1.pixels[i * grid1.width + j] 
                    - grid2.pixels[i * grid2.width + j]
                ) < 1e-12
            );
        }
    }

    free_grid(&grid1);
    free_grid(&grid2);
}

int main() {
    printf(">>>>>> Starting tests 'test_mandelbrot.c'...\n");
    printf("------ api-tests\n");
    test_init_grid_shape();
    test_init_grid_invalid_size();
    test_free_grid_sets_null();
    test_compute_mandelbrot_center_is_bounded();
    test_compute_mandelbrot_corners_are_unbounded();

    printf("----- helper-tests\n");
    test_complex_add();
    test_complex_square();
    test_get_complex_numbers();

    printf("----- threading-tests\n");
    test_compute_strip();
    test_compare_thread_results();

    printf("<<<<< All tests in 'test_mandelbrot.c' passed.\n");
    return 0;
}