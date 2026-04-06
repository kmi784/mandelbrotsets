#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/export.c"
#include "../src/mandelbrot.c"

static void test_save_grid_null_file(void) {
    MandelbrotGrid grid;
    assert(init_grid(&grid, GRID_DEBUG, -2.0, -1.0, 1.0, 1.0) == 0);

    assert(save_grid_txt(&grid, NULL) == 1);

    free_grid(&grid);
}

static void test_save_grid_basic_output(void) {
    MandelbrotGrid grid;
    assert(init_grid(&grid, GRID_DEBUG, -2.0, -1.0, 1.0, 1.0) == 0);

    // kleine Testwerte setzen
    for(unsigned int i = 0; i < grid.height; ++i) {
        for(unsigned int j = 0; j < grid.width; ++j) {
            grid.pixels[i * grid.width + j] = (double)(i + j);
        }
    }

    FILE* f = tmpfile();  // temporäre Datei
    assert(f != NULL);

    assert(save_grid_txt(&grid, f) == 0);

    rewind(f);

    // erste Zeile einlesen
    char buffer[1024];
    assert(fgets(buffer, sizeof(buffer), f) != NULL);

    // sollte mindestens Tabs enthalten
    assert(strchr(buffer, '\t') != NULL);

    fclose(f);
    free_grid(&grid);
}

static void test_save_grid_values_correct(void) {
    MandelbrotGrid grid;
    assert(init_grid(&grid, GRID_DEBUG, -2.0, -1.0, 1.0, 1.0) == 0);

    grid.pixels[0] = 1.5;
    grid.pixels[1] = 2.5;

    FILE* f = tmpfile();
    assert(f != NULL);

    assert(save_grid_txt(&grid, f) == 0);
    rewind(f);

    char buffer[1024];
    assert(fgets(buffer, sizeof(buffer), f) != NULL);
    assert(strstr(buffer, "1.500000") != NULL);
    assert(strstr(buffer, "2.500000") != NULL);

    fclose(f);
    free_grid(&grid);
}

int main(void) {
    printf(">>>>> Starting tests in 'test_export.c'...\n");
    printf("----- txt-export-tests\n");
    test_save_grid_null_file();
    test_save_grid_basic_output();
    test_save_grid_values_correct();

    printf("<<<<< All tests in 'test_export.c' passed.\n");
    return 0;
}