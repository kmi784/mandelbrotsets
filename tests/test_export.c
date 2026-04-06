#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/export.c"

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

static void test_save_image_ppm_null_file(void) {
    MandelbrotImage image;
    assert(init_image(&image, 10, 10) == 0);

    assert(save_image_ppm(&image, NULL) == 1);

    free_image(&image);
}

static void test_save_image_ppm_header_and_data(void) {
    MandelbrotImage image;
    assert(init_image(&image, 2, 2) == 0);

    /* feste Pixel setzen (RGB) */
    image.pixels[0] = 255; image.pixels[1] = 0;   image.pixels[2] = 0;   // rot
    image.pixels[3] = 0;   image.pixels[4] = 255; image.pixels[5] = 0;   // grün
    image.pixels[6] = 0;   image.pixels[7] = 0;   image.pixels[8] = 255; // blau
    image.pixels[9] = 255; image.pixels[10] = 255; image.pixels[11] = 255; // weiß

    FILE* f = tmpfile();
    assert(f != NULL);

    assert(save_image_ppm(&image, f) == 0);

    rewind(f);

    char buffer[64];

    /* Header prüfen */
    assert(fgets(buffer, sizeof(buffer), f) != NULL);
    assert(strncmp(buffer, "P6", 2) == 0);

    assert(fgets(buffer, sizeof(buffer), f) != NULL);
    assert(strstr(buffer, "2 2") != NULL);

    assert(fgets(buffer, sizeof(buffer), f) != NULL);
    assert(strncmp(buffer, "255", 3) == 0);

    fclose(f);
    free_image(&image);
}

static void test_save_image_png_null_path(void) {
    MandelbrotImage image;
    assert(init_image(&image, 10, 10) == 0);

    assert(save_image_png(&image, NULL) == 1);

    free_image(&image);
}

static void test_save_image_png_success(void) {
    MandelbrotImage image;
    assert(init_image(&image, 10, 10) == 0);

    /* einfache Daten */
    for(unsigned int i = 0; i < 10 * 10 * 3; ++i) {
        image.pixels[i] = (unsigned char)(i % 256);
    }

    const char* path = "test_output.png";

    assert(save_image_png(&image, path) == 0);

    /* Datei existiert und ist nicht leer */
    FILE* f = fopen(path, "rb");
    assert(f != NULL);

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    assert(size > 0);

    fclose(f);
    remove(path);  // cleanup

    free_image(&image);
}


int main(void) {
    printf(">>>>>> Starting tests in 'test_export.c'...\n");
    printf("------ txt-export-tests\n");
    test_save_grid_null_file();
    test_save_grid_basic_output();
    test_save_grid_values_correct();

    printf("------ image-export-tests\n");
    test_save_image_ppm_null_file();
    test_save_image_ppm_header_and_data();
    test_save_image_png_null_path();
    test_save_image_png_success();

    printf("<<<<<< All tests in 'test_export.c' passed.\n\n");
    return 0;
}